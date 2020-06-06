#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11
#include <Arduino.h>
#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
WidgetBridge bridge1(V1);
DHT dht(DHTPIN, DHTTYPE);
char auth[] = "NSWztMZmNsr3zLvaFLihOp6NF-SKfeo9";//clint id board2
char ssid[] = "new";
char pass[] = "12345678";
BLYNK_CONNECTED() {
  bridge1.setAuthToken("6UqgH13ON98rLmxl3b2mO89xjJUoIIG1");// server id
}
void setup() {
  Serial.begin(9600);
  Serial.println("Weather_Monitoring_System : Board_3");
  Serial.println("Board_3 id : NSWztMZmNsr3zLvaFLihOp6NF-SKfeo9");
  Serial.println("Board_1 id : 6UqgH13ON98rLmxl3b2mO89xjJUoIIG1");
  dht.begin();
  Wire.begin();
  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  while (!Serial);
}
void loop() {
  float h3 = dht.readHumidity();
  Serial.print(String("humidity: ") + h3);
  Blynk.virtualWrite(5, h3);
  bridge1.virtualWrite(V5, h3);
  delay(100);

  float t3 = dht.readTemperature();
  Serial.print(String("Temperature: ") + t3);
  Serial.println(" deg C");
  Blynk.virtualWrite(4, t3);
  bridge1.virtualWrite(V4, t3);
  Blynk.run();
  delay(1000);
}
