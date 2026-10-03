/*
  ESP32 Gyroscope Mouse
  ---------------------
  Hardware:
    ESP32
    MPU6050
    Push button

  Default wiring:
    MPU6050 VCC -> 3.3V
    MPU6050 GND -> GND
    MPU6050 SDA -> GPIO 21
    MPU6050 SCL -> GPIO 22
    Button one side -> GPIO 27
    Button other side -> GND

  Function:
    Move the ESP32/MPU6050 to control the Bluetooth mouse.
    Press the button on GPIO 27 to left-click.

  Required library:
    - ESP32 BLE Mouse (T-vK / ESP32-BLE-Mouse)
    - Adafruit MPU6050
    - Adafruit Unified Sensor
*/

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <BleMouse.h>

Adafruit_MPU6050 mpu;
BleMouse bleMouse("ESP32 Gyroscope Mouse", "Aravind", 100);

// ---------- Pins ----------
const int SDA_PIN = 21;
const int SCL_PIN = 22;
const int BUTTON_PIN = 27;

// ---------- Tuning ----------
const float GYRO_DEADZONE = 0.35f;   // Ignore small accidental movement
const float GYRO_SCALE = 7.0f;       // Increase for faster cursor
const int MAX_MOUSE_STEP = 18;       // Maximum movement per update

// Set true if the cursor moves in the opposite direction you want.
const bool INVERT_X = false;
const bool INVERT_Y = true;

// Button debounce
bool lastButtonState = HIGH;
bool stableButtonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_MS = 35;

// Gyroscope calibration offsets
float gyroXOffset = 0;
float gyroYOffset = 0;
float gyroZOffset = 0;

void calibrateGyroscope() {
  Serial.println("Keep the MPU6050 completely still...");
  delay(1500);

  const int samples = 300;
  float sumX = 0, sumY = 0, sumZ = 0;

  for (int i = 0; i < samples; i++) {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    sumX += gyro.gyro.x;
    sumY += gyro.gyro.y;
    sumZ += gyro.gyro.z;

    delay(5);
  }

  gyroXOffset = sumX / samples;
  gyroYOffset = sumY / samples;
  gyroZOffset = sumZ / samples;

  Serial.println("Calibration complete.");
  Serial.print("X offset: "); Serial.println(gyroXOffset, 5);
  Serial.print("Y offset: "); Serial.println(gyroYOffset, 5);
  Serial.print("Z offset: "); Serial.println(gyroZOffset, 5);
}

int convertToMouse(float value, bool invert) {
  if (fabs(value) < GYRO_DEADZONE) {
    return 0;
  }

  // Remove dead-zone while preserving direction.
  float direction = value > 0 ? 1.0f : -1.0f;
  float magnitude = fabs(value) - GYRO_DEADZONE;

  int movement = (int)(direction * magnitude * GYRO_SCALE);

  if (invert) {
    movement = -movement;
  }

  movement = constrain(movement, -MAX_MOUSE_STEP, MAX_MOUSE_STEP);
  return movement;
}

void handleButton() {
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_MS) {
    if (reading != stableButtonState) {
      stableButtonState = reading;

      if (stableButtonState == LOW && bleMouse.isConnected()) {
        bleMouse.click(MOUSE_LEFT);
        Serial.println("Left click");
      }
    }
  }

  lastButtonState = reading;
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Wire.begin(SDA_PIN, SCL_PIN);

  Serial.println();
  Serial.println("ESP32 Gyroscope Mouse");
  Serial.println("---------------------");

  if (!mpu.begin()) {
    Serial.println("ERROR: MPU6050 not found.");
    Serial.println("Check VCC, GND, SDA and SCL wiring.");
    while (true) {
      delay(1000);
    }
  }

  // Gyroscope configuration
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 detected.");

  calibrateGyroscope();

  bleMouse.begin();

  Serial.println("Bluetooth mouse started.");
  Serial.println("Pair with: ESP32 Gyroscope Mouse");
}

void loop() {
  handleButton();

  if (!bleMouse.isConnected()) {
    delay(20);
    return;
  }

  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  float correctedX = gyro.gyro.x - gyroXOffset;
  float correctedY = gyro.gyro.y - gyroYOffset;

  // Gyro X controls horizontal mouse movement.
  // Gyro Y controls vertical mouse movement.
  int mouseX = convertToMouse(correctedY, INVERT_X);
  int mouseY = convertToMouse(correctedX, INVERT_Y);

  if (mouseX != 0 || mouseY != 0) {
    bleMouse.move(mouseX, mouseY);
  }

  delay(10);
}
