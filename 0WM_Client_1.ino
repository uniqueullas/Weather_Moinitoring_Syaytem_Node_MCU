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
char auth[] = "5KmqoSVW1wPPzukyXAnq45qGBacmUjF9";//clint id board2
char ssid[] = "new";
char pass[] = "12345678";
BLYNK_CONNECTED() {
  bridge1.setAuthToken("6UqgH13ON98rLmxl3b2mO89xjJUoIIG1");// server id
}
void setup() {
  Serial.begin(9600);
  Serial.println("Weather_Monitoring_System : Board_2");
  Serial.println("Board_2 id : 5KmqoSVW1wPPzukyXAnq45qGBacmUjF9");
  Serial.println("Board_1 id : 6UqgH13ON98rLmxl3b2mO89xjJUoIIG1");
  dht.begin();
  Wire.begin();
  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  while (!Serial);
}
void loop() {
  float h2 = dht.readHumidity();
  Serial.print(String("humidity: ") + h2);
  Blynk.virtualWrite(3, h2);
  bridge1.virtualWrite(V3, h2);
  delay(100);

  float t2 = dht.readTemperature();
  Serial.print(String("Temperature: ") + t2);
  Serial.println(" deg C");
  Blynk.virtualWrite(2, t2);
  bridge1.virtualWrite(V2, t2);
  Blynk.run();
  delay(1000);
}
