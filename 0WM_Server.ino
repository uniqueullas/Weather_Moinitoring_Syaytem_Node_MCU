//[7:27 AM, 5/30/2020] Ullas M S: // 0WM_Server.ino
//
// shows how to use the BMP180MI library with the sensor connected using I2C.
//
// Copyright (c) 2018 Gregor Christandl
//
// connect the BMP180 to the Arduino like this:
// Arduino - BMC180
// 5V ------ VCC
// GND ----- GND
// SDA ----- SDA
// SCL ----- SCL
#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11   // DHT 11
#include <Arduino.h>
#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <BMP180MI.h>

#define I2C_ADDRESS 0x77

//create an BMP180 object using the I2C interface
BMP180I2C bmp180(I2C_ADDRESS);




WidgetBridge bridge1(V1);
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
char auth[] = "5KmqoSVW1wPPzukyXAnq45qGBacmUjF9";
// Your WiFi credentials.
// Set password to "" for open networks.
//char ssid[] = "do you want hotspot ?";
//char pass[] = "RUSSM655819222699";
char ssid[] = "new";
char pass[] = "12345678";
WidgetLCD lcd(V4);
int inPinVal ; //Virtual Pin Input
BLYNK_CONNECTED() {
  // Place the AuthToken of the second hardware here
  bridge1.setAuthToken("6UqgH13ON98rLmxl3b2mO89xjJUoIIG1");
}





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  WiFi.begin(ssid, pass);
  Blynk.config(auth);
  //wait for serial connection to open (only necessary on some boards)
  while (!Serial);

  Wire.begin();

  //begin() initializes the interface, checks the sensor ID and reads the calibration parameters.
  if (!bmp180.begin())
  {
    Serial.println("begin() failed. check your BMP180 Interface and I2C Address.");
    while (1);
  }

  //reset sensor to default parameters.
  bmp180.resetToDefaults();

  //enable ultra high resolution mode for pressure measurements
  bmp180.setSamplingMode(BMP180MI::MODE_UHR);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  delay(1000);
  float uh = dht.readHumidity();
  Serial.print("humidity: ");
  Serial.println(uh);
  Blynk.virtualWrite(8, dht.readHumidity());
  bridge1.virtualWrite(V8, dht.readHumidity());

  //start a temperature measurement
  if (!bmp180.measureTemperature())
  {
    Serial.println("could not start temperature measurement, is a measurement already running?");
    return;
  }

  //wait for the measurement to finish. proceed as soon as hasValue() returned true.
  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  Serial.print("Temperature: ");
  Serial.print(bmp180.getTemperature());
  Serial.println(" degC");
  Blynk.virtualWrite(4, bmp180.getTemperature());
  bridge1.virtualWrite(V4, bmp180.getTemperature());
  //start a pressure measurement. pressure measurements depend on temperature measurement, you should only start a pressure
  //measurement immediately after a temperature measurement.
  if (!bmp180.measurePressure())
  {
    Serial.println("could not start perssure measurement, is a measurement already running?");
    return;
  }

  //wait for the measurement to finish. proceed as soon as hasValue() returned true.
  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  Serial.print("Pressure: ");
  Serial.print(bmp180.getPressure());
  Serial.println(" Pa");
  Blynk.virtualWrite(5, bmp180.getPressure());
  bridge1.virtualWrite(V5, bmp180.getPressure());
}
