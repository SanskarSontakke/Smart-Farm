/* * SMART FARM - ESP32 MASTER
 * Handles WiFi, Blynk IoT connection, and requests data from the Arduino Mega (Slave) via I2C.
 */

#define BLYNK_TEMPLATE_ID "TMPL3r9t4nb73"
#define BLYNK_TEMPLATE_NAME "Smart Farm"
#define BLYNK_AUTH_TOKEN "wt_USwNac7Zjo4o9FJ9SiBnVX7EWkNqk"

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// --- Configuration ---
const char ssid[] = "Airtel_Orange";
const char pass[] = "pradnya223";

#define I2C_SDA_PIN 13
#define I2C_SCL_PIN 14
#define SLAVE_ADDRESS 11

// --- Global Variables ---
float currentHumidity = 0.0;
float currentTemperature = 0.0;
byte soilMoistureStatus = 0;
bool pumpState = false;

// User-defined thresholds from Blynk
int targetTemperature = 25; 
int targetHumidity = 60;

BlynkTimer timer;
WidgetTerminal terminal(V8);

// -----------------------------------------------------------------------------
// Request and Parse Data from the Arduino Mega (Slave) over I2C
// -----------------------------------------------------------------------------
void fetchSensorDataFromSlave() {
  Wire.requestFrom(SLAVE_ADDRESS, sizeof(float) * 2 + sizeof(byte));

  if (Wire.available() >= sizeof(float) * 2 + sizeof(byte)) {
    Wire.readBytes((byte*)&currentHumidity, sizeof(float));
    Wire.readBytes((byte*)&currentTemperature, sizeof(float));
    soilMoistureStatus = Wire.read();
  }
}

// -----------------------------------------------------------------------------
// Core Logic: Temperature, Humidity, and Soil Management
// -----------------------------------------------------------------------------
void processFarmLogic() {
  terminal.clear();
  fetchSensorDataFromSlave();

  // 1. Process Temperature
  terminal.print("TEMPERATURE : ");
  terminal.print(currentTemperature);
  terminal.println(" C");
  
  if (currentTemperature < targetTemperature) {
    terminal.println("Action -> Heater: ON | Cooler: OFF");
  } else if (currentTemperature > targetTemperature) {
    terminal.println("Action -> Heater: OFF | Cooler: ON");
  } else {
    terminal.println("Action -> Temp Optimal");
  }
  Blynk.virtualWrite(V0, currentTemperature);
  terminal.println("----------------------------------------");

  // 2. Process Humidity
  terminal.print("HUMIDITY : ");
  terminal.print(currentHumidity);
  terminal.println(" %");

  if (currentHumidity < targetHumidity) {
    terminal.println("Action -> Humidifier: ON | Dehumidifier: OFF");
  } else if (currentHumidity > targetHumidity) {
    terminal.println("Action -> Humidifier: OFF | Dehumidifier: ON");
  } else {
    terminal.println("Action -> Humidity Optimal");
  }
  Blynk.virtualWrite(V1, currentHumidity);
  terminal.println("----------------------------------------");

  // 3. Process Soil Moisture & Pump
  terminal.print("Soil Moisture Detected: ");
  terminal.println(soilMoistureStatus == 1 ? "YES (Wet)" : "NO (Dry)");
  
  Blynk.virtualWrite(V7, soilMoistureStatus);
  
  // Logic: If soil is dry (0), pump should turn ON (1)
  pumpState = !soilMoistureStatus; 
  terminal.print("Water Pump : ");
  terminal.println(pumpState ? "ON" : "OFF");
  terminal.println("========================================");
  
  terminal.flush();
}

// -----------------------------------------------------------------------------
// Blynk Callbacks (Triggered when user changes sliders in the app)
// -----------------------------------------------------------------------------
BLYNK_WRITE(V9) {
  targetTemperature = param.asInt();
}

BLYNK_WRITE(V10) {
  targetHumidity = param.asInt();
}

// -----------------------------------------------------------------------------
// Setup & Main Loop
// -----------------------------------------------------------------------------
void setup() {
  // Disable brownout detector to prevent ESP32 restarts during high power draw
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  
  Serial.begin(115200);
  
  // Initialize generic pins (Keep existing pin modes from original code)
  pinMode(2, OUTPUT);  digitalWrite(2, HIGH);
  pinMode(33, OUTPUT); digitalWrite(33, LOW);
  pinMode(15, OUTPUT); digitalWrite(15, HIGH);

  // Initialize I2C and Blynk
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup non-blocking timer to run processFarmLogic every 5 seconds (5000ms)
  timer.setInterval(5000L, processFarmLogic);
}

void loop() {
  Blynk.run();
  timer.run(); // Handles the 5-second interval without blocking the processor
}
