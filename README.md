# ESP32 Gyroscope Mouse

A Bluetooth HID mouse built using an ESP32 and MPU6050 gyroscope/accelerometer.

The project converts rotational movement of the MPU6050 into mouse cursor movement. A push button connected to GPIO 27 performs a left click.

## Features

- Wireless Bluetooth mouse
- ESP32-based
- MPU6050 gyroscope/accelerometer
- I2C sensor communication
- Adjustable sensitivity
- Gyroscope dead-zone
- Automatic gyroscope calibration at startup
- Push-button left click
- No physical mouse sensor required

## Hardware

| Component | Quantity |
|---|---:|
| ESP32 development board | 1 |
| MPU6050 | 1 |
| Push button | 1 |
| Jumper wires | As required |

## Wiring

### MPU6050 → ESP32

| MPU6050 | ESP32 |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### Push button → ESP32

| Button | ESP32 |
|---|---|
| One terminal | GPIO 27 |
| Other terminal | GND |

The code uses `INPUT_PULLUP`, so an external pull-up resistor is normally not required.

## Required Arduino libraries

Install:

1. **ESP32 BLE Mouse**
2. **Adafruit MPU6050**
3. **Adafruit Unified Sensor**

The ESP32 board package must also be installed in Arduino IDE.

## Upload

1. Connect the ESP32 to the computer.
2. Open `gyro_mouse.ino`.
3. Select the correct ESP32 board.
4. Select the correct COM port.
5. Upload the program.
6. Open Serial Monitor at `115200 baud`.
7. Keep the MPU6050 still during startup calibration.
8. On the computer's Bluetooth settings, pair with:
   **ESP32 Gyroscope Mouse**

## Controls

- Rotate/tilt the controller → move cursor
- Press GPIO 27 button → left click

## Sensitivity

Open the `.ino` file and change:

```cpp
const float GYRO_SCALE = 7.0f;
```

Examples:

```cpp
GYRO_SCALE = 4.0f;   // slower
GYRO_SCALE = 7.0f;   // default
GYRO_SCALE = 12.0f;  // faster
```

If the cursor moves because of tiny unwanted movements, increase:

```cpp
const float GYRO_DEADZONE = 0.35f;
```

For example:

```cpp
GYRO_DEADZONE = 0.50f;
```

## If movement is reversed

Change:

```cpp
const bool INVERT_X = false;
const bool INVERT_Y = true;
```

Try changing either value from `false` to `true`.

## Troubleshooting

### MPU6050 not detected

Check:

- VCC → 3.3V
- GND → GND
- SDA → GPIO 21
- SCL → GPIO 22

If your board uses different I2C pins, modify:

```cpp
Wire.begin(SDA_PIN, SCL_PIN);
```

### Cursor moves by itself

- Keep the MPU6050 still during startup.
- Increase `GYRO_DEADZONE`.
- Make sure the sensor is firmly mounted.
- Reduce `GYRO_SCALE`.

### Button does not work

Check:

- Button is connected between GPIO 27 and GND.
- The code uses `INPUT_PULLUP`.
- Do not connect the button to 3.3V for this wiring.

### Bluetooth mouse is not visible

- Reset the ESP32.
- Remove the previous `ESP32 Gyroscope Mouse` Bluetooth pairing.
- Wait for the device to advertise again.

## Project structure

```text
ESP32-Gyroscope-Mouse/
├── gyro_mouse.ino
├── README.md
└── WIRING.md
```

## Resume description

**ESP32 Gyroscope Mouse | ESP32, MPU6050, I2C, Bluetooth HID**

- Developed a wireless Bluetooth mouse using ESP32 and MPU6050 gyroscope data.
- Interfaced the MPU6050 over I2C and converted rotational motion into real-time cursor movement.
- Implemented gyroscope calibration, dead-zone filtering, sensitivity control, and push-button clicking.
