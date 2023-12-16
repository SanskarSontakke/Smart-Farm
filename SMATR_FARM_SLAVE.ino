#include <Wire.h>
#include <DHT.h>

#define DEVICE_ADDRESS 11
#define DHTPIN 2
#define DHTTYPE DHT11

bool Led_1_State = 0;
bool ledstate2 = 0;
bool soilMoisture;

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);
  Wire.begin(DEVICE_ADDRESS);  // Replace DEVICE_ADDRESS with the I2C address of the ESP32 slave
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendDHTData);
  dht.begin();
  pinMode(13, OUTPUT);
  pinMode(9, INPUT);
  pinMode(7, INPUT);
  digitalWrite(13, LOW);
  delay(500);
  Serial.println("Arduino Mega 2560 Actice");
  Serial.print("Device I2C Address : 0x");
  Serial.println(DEVICE_ADDRESS);
  Serial.println("Processor : ATmega2560 (Mega 2560) @ 12MHz");
  Serial.print("Connecting to ESP 32 Cam");
  while (digitalRead(9) == LOW) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" ");
  Serial.println("Successfully connected to ESP 32 Cam using I2C interface");
}

void loop() {
  delay(50);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  soilMoisture = digitalRead(7);
  digitalWrite(13, soilMoisture);
  if (soilMoisture == 0) {
    digitalWrite(6, HIGH);
  } else if (soilMoisture == 1) {
    digitalWrite(6, LOW);
  }
}

void receiveEvent() {
  String receivedData = "";

  while (Wire.available()) {
    char c = Wire.read();
    receivedData += c;
  }

  if (receivedData == "HEATER_0/1_ON") {
    Led_1_State = !Led_1_State;
    Serial.print("Heater :");
    Serial.println("ON");
  }
  if (receivedData == "HEATER_0/1_OFF") {
    Led_1_State = !Led_1_State;
    Serial.print("Heater :");
    Serial.println("OFF");
  }
}

void sendDHTData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Wire.write((byte*)&humidity, sizeof(float));     // Send humidity data
  Wire.write((byte*)&temperature, sizeof(float));  // Send temperature data
  Wire.write(soilMoisture);
}
