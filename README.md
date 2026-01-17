# 🤖 Autonomous Firefighting Robot with Thermal Verification & GSM Alerting

An intelligent, autonomous robot designed to detect, track, and extinguish fire sources. This project stands out by using **dual-verification logic**: it combines IR flame sensors for navigation with an **MLX90614 Contactless Infrared Thermometer** to confirm heat signatures, significantly reducing false positives from sunlight or lamps.



---

## ✨ Features
* **Flame Navigation:** Uses a 3-way IR sensor array to locate and steer toward a fire source.
* **Contactless Thermal Sensing:** Verifies fire using an MLX90614 sensor (I2C) to measure actual surface temperature.
* **Automatic Extinguishing:** Controls a water pump via a 5V relay with a servo-assisted sweeping nozzle.
* **GSM IoT Alerts:** Sends a real-time "Fire Detected" SMS via the SIM800L module to a designated mobile number.
* **Safety Logic:** Includes an SMS cooldown timer to prevent network flooding and a local buzzer alarm.

---

## 🛠️ Hardware Components
* **Microcontroller:** Arduino Uno / Nano
* **Temperature Sensor:** MLX90614 (Contactless Infrared Thermometer)
* **GSM Module:** SIM800L (Quad-band)
* **Motor Driver:** L298N Dual H-Bridge
* **Flame Sensors:** 3x IR Flame Sensors
* **Actuators:** 1x MG995/SG90 Servo, 1x 5V Submersible Pump
* **Power:** 7.4V Li-ion battery (Logic/Motors) + 3.7V Li-ion (Dedicated for GSM)

---

## 📌 Pin Mapping

| Component | Arduino Pin | Function |
| :--- | :--- | :--- |
| **L298N IN1/IN2** | D2, D3 | Left Motor Control |
| **L298N IN3/IN4** | D4, D5 | Right Motor Control |
| **Relay** | D6 | Water Pump Switch |
| **SIM800L TX/RX** | D7, D8 | Software Serial Communication |
| **IR Left** | D9 | Navigation (Left) |
| **IR Right** | D10 | Navigation (Right) |
| **IR Center** | D11 | Fire Proximity Trigger |
| **Servo** | D12 | Nozzle Sweep Control |
| **Buzzer** | D13 | Audible Alarm |
| **MLX90614** | A4 (SDA), A5 (SCL) | I2C Temperature Data |

---

## 🚀 Installation & Setup

### 1. Library Installation
Ensure you have the following libraries installed in your Arduino IDE:
* `Adafruit_MLX90614`
* `Servo`
* `SoftwareSerial`

### 2. Configuration
Open the `.ino` file and update the phone number to your own:
```cpp
SIM800L.println("AT+CMGS=\"+911234567890\""); // Replace with your number

