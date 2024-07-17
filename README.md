
# ESP32-Based OTP Solenoid Lock System

# Overview
This project involves developing a secure IoT-based access control system using an ESP32 microcontroller. The system generates and sends One-Time Passwords (OTPs) via email, which users enter through a keypad to unlock a solenoid lock. A VL53L0X Time-of-Flight (TOF) sensor is used for distance measurement to trigger OTP generation.

# Features
- WiFi connectivity for sending OTP emails
- Keypad interface for OTP entry
- Solenoid lock control
- Distance measurement using VL53L0X TOF sensor
- Email notification with generated OTP

# Components
- ESP32 microcontroller
- 4x4 Keypad
- Solenoid lock
- Relay module
- VL53L0X TOF sensor
- Additional components: LEDs, resistors, connecting wires
- 
# Installation

1. **Setup the Hardware**:
   - Connect the keypad to the ESP32.
   - Connect the solenoid lock to the relay module, and the relay module to the ESP32.
   - Connect the VL53L0X sensor to the ESP32 using I2C.

2. **Install the Required Libraries**:
   - Install the following libraries via the Arduino Library Manager or download them from GitHub:
     - `WiFi.h`
     - `ESP_Mail_Client.h`
     - `Keypad.h`
     - `Adafruit_VL53L0X.h`
     - `Wire.h`

3. **Configure the Code**:
   - Replace the placeholder WiFi credentials, email credentials, and recipient email addresses in the code.
   - Upload the code to the ESP32 using the Arduino IDE.

# Usage
1. Power up the system.
2. Connect the ESP32 to WiFi.
3. Trigger OTP generation by pressing '*' on the keypad or by an object approaching the VL53L0X sensor.
4. Enter the received OTP on the keypad to unlock the solenoid lock.


## References
- [ESP32 Official Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
- [ESP32 Overview and Features](https://www.espressif.com/en/products/socs/esp32/overview)
- [ESP32 WiFi Library](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/wifi/index.html)
- [Setting Up WiFi on ESP32](https://randomnerdtutorials.com/esp32-connect-wifi-password-setting/)
- [Arduino Keypad Library](https://playground.arduino.cc/code/keypad/)
- [Using Keypads with ESP32](https://randomnerdtutorials.com/esp32-esp8266-keypad-lcd/)
- [ESP32 Email Sending Tutorial](https://randomnerdtutorials.com/esp32-send-email-smtp-gmail/)
- [Generating OTPs in Arduino](https://www.geeksforgeeks.org/generate-a-one-time-password-otp-using-python/)
- [Adafruit VL53L0X Library](https://github.com/adafruit/Adafruit_VL53L0X)
- [VL53L0X Arduino Library and Example Code](https://learn.adafruit.com/adafruit-vl53l0x-micro-lidar-distance-sensor-breakout/arduino-code)

