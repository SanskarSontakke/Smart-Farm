#define BLYNK_TEMPLATE_ID "TMPL3r9t4nb73"
#define BLYNK_TEMPLATE_NAME "Smart Farm"
#define BLYNK_AUTH_TOKEN "wt_USwNac7Zjo4o9FJ9SiBnVX7EWkNqk"
#define DHTTYPE DHT11
#define DHTPIN 16
#define DEVICE_ADDRESS 11

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

char ssid[] = "Airtel_Orange";
char pass[] = "pradnya223";
String Value;
bool pumpState = 0;
int lastSliderValue = 2;
int lastSliderValue_2 = 60;
int soilMoisture = 0;
float humidity = 27;
float temperature = 60;

WidgetTerminal terminal(V8);

void requestDHTData() {
  Wire.requestFrom(DEVICE_ADDRESS, sizeof(float) * 2 + sizeof(bool));  // Request data from the slave

  if (Wire.available() >= sizeof(float) * 2) {
    Wire.readBytes((byte*)&humidity, sizeof(float));     // Read humidity data
    Wire.readBytes((byte*)&temperature, sizeof(float));  // Read temperature data
    soilMoisture = Wire.read();
  }
}
//---------------------------Temperature Values Set Code---------------------------//
void temperatureValuesSet() {
  if (temperature < lastSliderValue) {
    terminal.println("Heater : ON");
    terminal.println("Cooler : OFF");
  } else if (temperature > lastSliderValue) {
    terminal.println("Heater : OFF");
    terminal.println("Cooler : ON");
  }
  terminal.print("TEMPERATURE : ");
  terminal.print(temperature);
  terminal.println(" C");
  Blynk.virtualWrite(V0, temperature);
  terminal.println("--------------------------------------------------------------------------------");
}
//---------------------------Temperature Values Set Code End---------------------------//
//--------------------------Humidity Values Set Code---------------------------//
void humidityValuesSet() {
  if (humidity < lastSliderValue_2) {
    terminal.println("Humidifier : ON");
    terminal.println("Dehumidifier : OFF");
  } else if (humidity > lastSliderValue_2) {
    terminal.println("Humidifier : OFF");
    terminal.println("Dehumidifier : ON");
  }

  terminal.print("HUMIDITY : ");
  terminal.print(humidity);
  terminal.println(" %");
  Blynk.virtualWrite(V1, humidity);
  terminal.println("--------------------------------------------------------------------------------");
}
//--------------------------Humidity Values Set Code End---------------------------//
//--------------------------Soil Moisture Read Code---------------------------//
void soilMoistureRead() {
  terminal.print("Soil Moisture: ");
  if (soilMoisture == 0) {
    terminal.println("NO");
  } else if (soilMoisture == 1) {
    terminal.println("YES");
  }
  Serial.println(soilMoisture);
  Blynk.virtualWrite(V7, soilMoisture);
  pumpState = !soilMoisture; 
  terminal.print("Pump : ");
  if (pumpState == 1) {
    terminal.println("ON");
  }else if (pumpState == 0) {
    terminal.println("OFF");
  }

  terminal.println("--------------------------------------------------------------------------------");
}
//--------------------------Soil Moisture Read Code End---------------------------//
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  delay(3000);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  Serial.begin(115200);
  Wire.begin(13, 14);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  terminal.clear();
  requestDHTData();
  //---------------------------Main Loop Code---------------------------//
  temperatureValuesSet();
  humidityValuesSet();
  soilMoistureRead();
  //---------------------------Main Loop Code End---------------------------//
  delay(5000);
}

//---------------------------Temperature Slider Control---------------------------//
BLYNK_WRITE(V9) {
  int sliderValue = param.asInt();

  lastSliderValue = sliderValue;
}
//---------------------------Temperature Slider Control End---------------------------//
//---------------------------Humidity Slider Control---------------------------//
BLYNK_WRITE(V10) {
  int sliderValue_2 = param.asInt();

  lastSliderValue_2 = sliderValue_2;
}
//---------------------------Humidity Slider Control End---------------------------//