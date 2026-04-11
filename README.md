# 🌱 Smart-Farm IoT System

A robust, dual-microcontroller Smart Farming system designed to monitor and automate environmental conditions for optimal plant growth. 

This project uses an **ESP32 (Master)** connected to the internet via WiFi to communicate with the **Blynk IoT platform**, and an **Arduino Mega 2560 (Slave)** to handle local sensor polling (DHT11 and Soil Moisture) and hardware control. The two boards communicate seamlessly using the **I2C protocol**.

## ✨ Features
* **Live IoT Dashboard:** Monitor Temperature, Humidity, and Soil Moisture from anywhere using the Blynk app.
* **Automated Climate Control:** * Compares real-time temperature against a user-defined threshold to trigger a Heater or Cooler.
  * Compares real-time humidity against a threshold to trigger a Humidifier or Dehumidifier.
* **Smart Irrigation:** Reads digital soil moisture levels and automates a water pump accordingly.
* **Master-Slave Architecture:** Offloads heavy sensor polling to the Arduino Mega, leaving the ESP32 free to handle secure WiFi and WebSocket connections to the Blynk Cloud.

## 🛠️ Hardware Requirements
* **Master Board:** ESP32 (or ESP32-CAM)
* **Slave Board:** Arduino Mega 2560
* **Sensors:** * DHT11 (Temperature & Humidity Sensor)
  * Digital Soil Moisture Sensor
* **Actuators (Relays/LEDs for simulation):**
  * Water Pump, Heater, Cooler, Humidifier, Dehumidifier
* **Wiring:** I2C connection (SDA/SCL) between the ESP32 and Arduino Mega.

## 📱 Blynk Configuration
Configure your Blynk Web Dashboard / Mobile App with the following Virtual Pins:
* **`V0` (Display):** Temperature (°C)
* **`V1` (Display):** Humidity (%)
* **`V7` (Display/LED):** Soil Moisture Status
* **`V8` (Terminal):** System Event Logs
* **`V9` (Slider):** Target Temperature Threshold 
* **`V10` (Slider):** Target Humidity Threshold

## 🚀 Getting Started
1. Install the required libraries in your Arduino IDE: `Blynk`, `DHT sensor library`, and `Adafruit Unified Sensor`.
2. Update the `SMART_FARM.ino` file with your specific **WiFi Credentials** and **Blynk Auth Token**.
3. Flash `SMATR_FARM_SLAVE.ino` to the Arduino Mega.
4. Flash `SMART_FARM.ino` to the ESP32.
5. Connect the I2C lines (ESP32 pins 13/14 to Mega SDA/SCL) and power them up!
