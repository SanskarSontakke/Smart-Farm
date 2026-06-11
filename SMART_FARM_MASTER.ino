#define DHTTYPE DHT11
#define DHTPIN 16
#define DHTTYPE DHT11

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <WiFiClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

const char *ssid = "Airtel_Orange";
const char *password = "pradnya223";

DHT dht(DHTPIN, DHTTYPE);

// Define LEDs and button
const int ledPin1 = 14;    // GPIO pin for LED 1
const int ledPin2 = 15;    // GPIO pin for LED 2
const int buttonPin = 12;  // GPIO pin for the external button
int sliderValue1;
int sliderValue2;
// Variable to store button state
int buttonState = 0;

// Create an instance of the web server
AsyncWebServer server(80);

void setup() {
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  Wire.begin(13, 14);
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int sliderValue1;
    int sliderValue2;

    String html = "<html lang=\"en\">";
    html += "<head>";
    html += "<meta charset=\"UTF-8\">";
    html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    html += "<title>Document</title>";
    html += "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\"        integrity=\"sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC\" crossorigin=\"anonymous\">";
    html += "<link href=\"index.css\" rel=\"stylesheet\" />";
    html += "</head>";
    html += "<body>";
    html += "<div class=\"container-fluid\">";
    html += "<div class=\"row\">";
    html += "<div class=\"col-12\">";
    html += " <nav class=\"navbar navbar-expand-lg navbar-light bg-danger\">";
    html += "<div class=\"container-fluid\">";
    html += "<a class=\"navbar-brand text-white\" href=\"index.html\">SANSAKR</a>";
    html += "<button class=\"navbar-toggler\" type=\"button\" data-bs-toggle=\"collapse\" data - bs - target = \"#navbarSupportedContent\" aria - controls = \"navbarSupportedContent\" aria - expanded = \"false\" aria - label = \"Toggle navigation\" >";
    html += "<span class=\"navbar-toggler-icon\"></span>";
    html += "</button>";
    html += "<div class=\"collapse navbar-collapse\" id=\"navbarSupportedContent\">";
    html += "<ul class=\"navbar-nav me-auto mb-2 mb-lg-0\">";
    html += "<li class=\"nav-item\">";
    html += "<a class=\"nav-link active text-white\" aria-current=\"page\" href=\"#\">Home</a>";
    html += "</li>";
    html += "</ul>";
    html += "</div>";
    html += "</div>";
    html += "</nav>";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"row mt-3\">";
    html += "<h2 class=\"text-center text-primary\">Little School Public School , Nanded</h2>";
    html += "<hr>";
    html += "<div class=\"col-6\">";
    html += "<div id=\"gauge1\"></div>";
    html += "</div>";
    html += "<div class=\"col-6\">";
    html += "<div id=\"gauge2\"></div>";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"row\">";
    html += "<div class=\"col-6 text-center\">";

    html += "<p>Slider 1 Value: <span id='slider1-value'>0</span></p>";
    html += "<input type='range' id='slider1' min='0' max='1023' value='0' onchange='updateSliderValue(1)'>";
    html += "<p>Slider 2 Value: <span id='slider2-value'>0</span></p>";
    html += "<input type='range' id='slider2' min='0' max='1023' value='0' onchange='updateSliderValue(2)'>";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"row mt-5\">";
    html += "<div class=\"col-6 \">";
    html += "<div class=\"red\">";
    html += "</div>";
    html += "</div>";
    html += "<div class=\"col-6\">";
    html += "<div class=\"gray\"> ";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "</div>";
    html += "</body>";
    html += "</html>";

    html += "<script src=\"https://bpegm.in/raphael.min.js\"></script>";
    html += "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/justgage/1.6.1/justgage.min.js\"    integrity=\"sha512-QeISJM4NCnUgZl5++o/2d4FwppF+Hh62RbNeA9paupnQvq5KAVvf2ZN3KD99EDoqcSHi1kbG13JMyRXDKBQBSw==\"    crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>";
    html += "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/justgage/1.6.1/justgage.js\"  integrity=\"sha512-tWxvieYDBICxbDkkAlYex1Qu6dzMfPBzpzdYd+eu9IuBYSeUSpGn8W6AUQvbBfoaiFcDxXFl6qURZxFcJVFIRg==\"    crossorigin=\"anonymous\" referrerpolicy=\"no-referrer\"></script>";
    html += "</script>";

    // html += "<script>";
    // html += "setInterval(() => {";
    // html += "gauge1.refresh(" + String(temperature) + ");";
    // html += "gauge2.refresh(" + String(humidity) + ");";
    // html += "}, 1000)";
    // html += "</script>";

    html += "<script>";
    html += "function updateSliderValue(sliderId) {";
    html += "  var sliderValue = document.getElementById('slider' + sliderId).value;";
    html += "  document.getElementById('slider' + sliderId + '-value').innerText = sliderValue;";
    html += "}";
    html += "</script>";

    // html += "<script>";
    // html += "function updateGuage(temp,hum){";
    // html += "var gauge1 = new JustGage({";
    // html += "id: \"gauge1\",value: temp ,min: 0,max: 100,decimals: 2,gaugeWidthScale: 0.6,label: \"Temp\",labelFontColor: '#FF0000',levelColors: [\"#FF0000\", \"#FF0000\", \"#00FF00\", \"#FF0000\", \"#FF0000\"],pointer: true,";
    // html += "});";
    // html += "var gauge2 = new JustGage({";
    // html += "id: \"gauge2\",value: hum ,min: 0,max: 100,decimals: 2,gaugeWidthScale: 0.6,label: \"Humidity\",labelFontColor: '#0000FF',levelColors: [\"#FF0000\", \"#FF0000\", \"#00FF00\", \"#FF0000\", \"#FF0000\"],pointer: true";
    // html += "});";
    // html += "}";

    // // html += "setInterval(() => {";
    // //html += " updateGuage("+ String(temperature) + "," + String(humidity) + ")";
    // // html += "}, 1000)";

    // // html += "var gauge1 = new JustGage({";
    // // html += "id: \"gauge1\",value:" + String(temperature) + ",min: 0,max: 100,decimals: 2,gaugeWidthScale: 0.6,label: \"Temp\",labelFontColor: '#FF0000',levelColors: [\"#FF0000\", \"#FF0000\", \"#00FF00\", \"#FF0000\", \"#FF0000\"],pointer: true,";
    // // html += "});";
    // // html += "var gauge2 = new JustGage({";
    // // html += "id: \"gauge2\",value:" + String(humidity) + ",min: 0,max: 100,decimals: 2,gaugeWidthScale: 0.6,label: \"Humidity\",labelFontColor: '#0000FF',levelColors: [\"#FF0000\", \"#FF0000\", \"#00FF00\", \"#FF0000\", \"#FF0000\"],pointer: true";
    // // html += "});";
    // html += "</script>";


    // html += "";
    // html += "";
    // html += "";
    // html += "";
    // html += "";
    // html += "";
    // html += "";

    request->send(200, "text/html", html);
  });

  server.on("/updateSlider", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int sliderValue1;
    int sliderValue2;

    int sliderId = request->arg("sliderId").toInt();
    int value = request->arg("value").toInt();

    if (sliderId == 1) {
      sliderValue1 = value;
      Serial.print("Slider 1 Value: ");
      Serial.println(sliderValue1);

    } else if (sliderId == 2) {
      sliderValue2 = value;
      Serial.print("Slider 2 Value: ");
      Serial.println(sliderValue2);
    }

    request->send(200, "text/plain", "Slider updated");
  });

  // Start server
  server.begin();
}

void loop() {
  Serial.println(dht.readTemperature());
  Serial.println(dht.readHumidity());
  if (isnan(dht.readTemperature())) {
    Serial.println("ERROR");
  } else {
  }
  if (isnan(dht.readHumidity())) {
    Serial.println("ERROR");
  } else {
  }

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int sliderValue1;
    int sliderValue2;

    String html = "<script>";
    html += "function updateGuage(temp,hum){";
    html += "var gauge1 = new JustGage({";
    html += "id: \"gauge1\",value: temp ,min: 0,max: 100,decimals: 2,gaugeWidthScale: 0.6,label: \"Temp\",labelFontColor: '#FF0000',levelColors: [\"#FF0000\", \"#FF0000\", \"#00FF00\", \"#FF0000\", \"#FF0000\"],pointer: true,";
    html += "});";
    html += "var gauge2 = new JustGage({";
    html += "id: \"gauge2\",value: hum ,min: 0,max: 100,decimals: 2,gaugeWidthScale: 0.6,label: \"Humidity\",labelFontColor: '#0000FF',levelColors: [\"#FF0000\", \"#FF0000\", \"#00FF00\", \"#FF0000\", \"#FF0000\"],pointer: true";
    html += "});";
    html += "}";

    // html += "setInterval(() => {";
    //html += " updateGuage("+ String(temperature) + "," + String(humidity) + ")";
    // html += "}, 1000)";

    // html += "var gauge1 = new JustGage({";
    // html += "id: \"gauge1\",value:" + String(temperature) + ",min: 0,max: 100,decimals: 2,gaugeWidthScale: 0.6,label: \"Temp\",labelFontColor: '#FF0000',levelColors: [\"#FF0000\", \"#FF0000\", \"#00FF00\", \"#FF0000\", \"#FF0000\"],pointer: true,";
    // html += "});";
    // html += "var gauge2 = new JustGage({";
    // html += "id: \"gauge2\",value:" + String(humidity) + ",min: 0,max: 100,decimals: 2,gaugeWidthScale: 0.6,label: \"Humidity\",labelFontColor: '#0000FF',levelColors: [\"#FF0000\", \"#FF0000\", \"#00FF00\", \"#FF0000\", \"#FF0000\"],pointer: true";
    // html += "});";
    html += "</script>";

    request->send(200, "text/html", html);
  });

  delay(1000);
}
