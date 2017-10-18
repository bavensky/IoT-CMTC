#include <WiFi.h>
#include <HTTPClient.h>

#include "DHT.h"        // เรียกใช้งานไลบรารี่ DHT.h
#define DHTPIN 14       // เชื่อมต่อเซ็นเซอร์ขา 14
#define DHTTYPE DHTxx   // เลือกชนิดเซ็นเซอร์แบบ DHT
DHT dht(DHTPIN, DHTTYPE);

#define YOUR_WIFI_NAME "xxxxxxxx"   // ชื่อไวไฟที่ต้องการเชื่อมต่อ
#define YOUR_WIFI_PASS "xxxxxxxx"   // รหัสไวไฟที่ต้องการเชื่อมต่อ
#define YOUR_API_KEY   "xxxxxxxx"   // กรอก API key ที่ได้จากเว็บ Thingspeak



// ฟังก์ชันส่งข้อมูลเข้าเว็บ Thingspeak โดยรับพารามิเตอร์ชื่อ url
void sent2Thingspeak(String url) {
  // ประกาศออบเจค และส่งค่าแบบ HTTP
  HTTPClient http;
  Serial.println("Send data" + url);
  http.begin(url);

  // รับข้อมูลจากเว็บเพื่อตรวจสอบว่าส่งข้อมูลได้หรือไม่
  int httpCode = http.GET();
  if (httpCode == 200) {
    Serial.println("Success. code = " + String(httpCode));
  } else {
    Serial.println("Fail. error code = " + String(httpCode));
  }
}



void setup() {
  Serial.begin(115200);             // กำหนดความเร็วในการติดต่อสื่อสาร
  pinMode(LED_BUILTIN, OUTPUT);     // กำหนดให้ LED บนบอร์ดเป็น OUTPUT
  dht.begin();                      // ประกาศใช้งาน dht library

  Serial.print("Connecting to ");
  Serial.println(YOUR_WIFI_NAME);   // แสดงชื่อไวไฟที่เชื่อมต่อใน Serial monitor


  WiFi.mode(WIFI_STA);              // เชื่อมต่อไวไฟที่กำหนด
  WiFi.begin(YOUR_WIFI_NAME, YOUR_WIFI_PASS);

  // วนรอบเชื่อมต่อไวไฟเรื่อย ๆ จนกว่าจะติด
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }


  // แสดงข้อความใน Serial monitor เมื่อเชื่อมต่อเสร็จสมบูรณ์
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());     // แสดง IP ของบอร์ด
}



void loop() {
  float h = dht.readHumidity();     // อ่านค่าอุณหภูมิจาก dht11
  float t = dht.readTemperature();  // อ่านค่าคามชื้นจาก dht11

  // แสดงผลค่าอุณหภูมิ และความชื้นสัมพัทธ์ผ่าน serial monitor
  Serial.print("Humidity: ");       
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");


  // สร้าง url ที่จะส่งไปยัง thingspeak
  String url =  "http://api.thingspeak.com/update?api_key=" + String(YOUR_API_KEY)
                + "&field1=" + String(t) + "&field2=" + String(h);

  sent2Thingspeak(url); // ส่งค่าไปยังเว็บ Thingspeak.com
  delay(20000);         //  หน่วงเวลา 20 วินาที
}
