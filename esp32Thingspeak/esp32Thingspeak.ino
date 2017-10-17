#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_STA_NAME "esp32Tester"
#define WIFI_STA_PASS "AJapirak"

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {

  int number = random(0, 100);
  String url =  "http://api.thingspeak.com/update?api_key=5T4WXGZFE1PZPS2K&field1=" + String(number);

  HTTPClient http;
  Serial.println("Send data" + String(url));
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode == 200) {
    Serial.println("Success. code = " + String(httpCode));
  } else {
    Serial.println("Fail. error code = " + String(httpCode));
  }
  Serial.println("END");
  delay(20000);

}
