/* * SMART FARM - ARDUINO MEGA 2560 SLAVE
 * Handles local sensors (DHT11, Soil Moisture) and responds to I2C requests from ESP32.
 */

#include <Wire.h>
#include <DHT.h>

// --- Configuration ---
#define DEVICE_ADDRESS 11
#define DHTPIN 2
#define DHTTYPE DHT11

// Pin Definitions
#define SOIL_MOISTURE_PIN 7
#define ESP_READY_PIN 9
#define PUMP_RELAY_PIN 6
#define STATUS_LED_PIN 13

DHT dht(DHTPIN, DHTTYPE);

// --- Global Sensor Variables (Volatile for safe use in Interrupts) ---
volatile float currentHumidity = 0.0;
volatile float currentTemperature = 0.0;
volatile byte soilMoistureState = 0;

bool heaterState = false;
unsigned long lastDHTReadTime = 0;

// -----------------------------------------------------------------------------
// Setup
// -----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  
  // Initialize Pins
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(ESP_READY_PIN, INPUT);
  
  digitalWrite(STATUS_LED_PIN, LOW);
  digitalWrite(PUMP_RELAY_PIN, LOW);

  // Initialize Sensors & I2C
  dht.begin();
  Wire.begin(DEVICE_ADDRESS); 
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendSensorData);

  // Startup Logs
  Serial.println("\n--- Arduino Mega 2560 Active ---");
  Serial.print("Device I2C Address: 0x");
  Serial.println(DEVICE_ADDRESS, HEX);
  
  // Wait for ESP32 to signal readiness (Optional hardware handshake)
  Serial.print("Waiting for ESP32 Master...");
  while (digitalRead(ESP_READY_PIN) == LOW) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nSuccessfully connected to ESP32 via I2C!");
}

// -----------------------------------------------------------------------------
// Main Loop
// -----------------------------------------------------------------------------
void loop() {
  // 1. Read DHT11 non-blockingly (DHT11 is slow, only read every 2 seconds)
  if (millis() - lastDHTReadTime >= 2000) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    
    // Only update if reading was successful (not NaN)
    if (!isnan(h) && !isnan(t)) {
      currentHumidity = h;
      currentTemperature = t;
    }
    lastDHTReadTime = millis();
  }

  // 2. Read Soil Moisture Sensor
  soilMoistureState = digitalRead(SOIL_MOISTURE_PIN);
  
  // Update onboard LED to reflect moisture state
  digitalWrite(STATUS_LED_PIN, soilMoistureState);

  // 3. Control local hardware (Pump/Valve)
  if (soilMoistureState == LOW) {
    // Soil is Dry -> Turn ON water
    digitalWrite(PUMP_RELAY_PIN, HIGH);
  } else {
    // Soil is Wet -> Turn OFF water
    digitalWrite(PUMP_RELAY_PIN, LOW);
  }
}

// -----------------------------------------------------------------------------
// I2C Interrupt: Triggered when ESP32 requests data
// Note: Keep this extremely short! No delays or sensor reads inside here.
// -----------------------------------------------------------------------------
void sendSensorData() {
  // Send data sequentially: Humidity (4 bytes), Temp (4 bytes), Moisture (1 byte)
  Wire.write((byte*)&currentHumidity, sizeof(float));
  Wire.write((byte*)&currentTemperature, sizeof(float));  
  Wire.write(soilMoistureState);
}

// -----------------------------------------------------------------------------
// I2C Interrupt: Triggered when ESP32 sends commands
// Note: Avoid using Arduino 'String' objects inside interrupts to prevent crashes.
// -----------------------------------------------------------------------------
void receiveEvent(int howMany) {
  char cmdBuffer[32];
  int i = 0;
  
  while (Wire.available() && i < sizeof(cmdBuffer) - 1) {
    cmdBuffer[i++] = Wire.read();
  }
  cmdBuffer[i] = '\0'; // Null terminate the string

  // Process incoming commands
  if (strcmp(cmdBuffer, "HEATER_0/1_ON") == 0) {
    heaterState = true;
    Serial.println("Command Received: Heater ON");
  } 
  else if (strcmp(cmdBuffer, "HEATER_0/1_OFF") == 0) {
    heaterState = false;
    Serial.println("Command Received: Heater OFF");
  }
}
