#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"
#define DHTPIN            14
#define DHTTYPE           DHT11
DHT dht(DHTPIN, DHTTYPE);
int dataTemp = 0, dataHumid = 0;
unsigned long preMillis = 0;


#define WIFI_STA_NAME "xxxxxxxx"
#define WIFI_STA_PASS "xxxxxxxx"
#define MQTT_SERVER   "xxxxxxxx"
#define MQTT_PORT     xxxx
#define MQTT_USERNAME "xxxxxxxx"
#define MQTT_PASSWORD "xxxxxxxx"
#define MQTT_NAME     "ESP32_CMTC"
#define LED_PIN 12
char msg[100];



WiFiClient client;
PubSubClient mqtt(client);



void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String topic_str = topic, payload_str = (char*)payload;
  Serial.println("[" + topic_str + "]: " + payload_str);

  digitalWrite(LED_PIN, (payload_str == "ON") ? HIGH : LOW);
  digitalWrite(LED_BUILTIN, (payload_str == "ON") ? HIGH : LOW);
}



void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();

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

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(callback);
}



void loop() {
  if (mqtt.connected() == false) {
    Serial.print("MQTT connection... ");
    if (mqtt.connect(MQTT_NAME, MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");
      mqtt.subscribe("/esp32/#");
    } else {
      Serial.println("failed");
      delay(5000);
    }
  } else {
    unsigned long curMillis = millis();
    if (curMillis - preMillis >= 5000) {
      preMillis = curMillis;
      int t = dht.readTemperature();
      snprintf (msg, 75, "%d", t);
      mqtt.publish("/esp32/temp", msg);

      int h = dht.readHumidity();
      snprintf (msg, 75, "%d", h);
      mqtt.publish("/esp32/humid", msg);

    }
    mqtt.loop();
  }
}
