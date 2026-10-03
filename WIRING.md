# Wiring

## MPU6050

```text
MPU6050       ESP32
--------------------
VCC     ----> 3.3V
GND     ----> GND
SDA     ----> GPIO 21
SCL     ----> GPIO 22
```

## Button

```text
Button terminal 1 ----> GPIO 27
Button terminal 2 ----> GND
```

The firmware uses the ESP32 internal pull-up resistor:

```cpp
pinMode(BUTTON_PIN, INPUT_PULLUP);
```

Therefore the button is active LOW.
