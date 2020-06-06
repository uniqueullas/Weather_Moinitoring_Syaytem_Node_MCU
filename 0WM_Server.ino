//0WM_Server.ino
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <BMP180MI.h>
#define I2C_ADDRESS 0x77
BMP180I2C bmp180(I2C_ADDRESS);

#include "DHT.h"
#define DHTPIN 14
#define DHTTYPE DHT11   // DHT 11
#include <Arduino.h>
DHT dht(DHTPIN, DHTTYPE);

#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = "6UqgH13ON98rLmxl3b2mO89xjJUoIIG1";

char ssid[] = "new";
char pass[] = "12345678";


int status;
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);    // The begin call takes the width and height. This
  lcd.init();          // Should match the number provided to the constructor.
  lcd.backlight();     // Turn on the backlight.
  lcd.setCursor(0, 0); // Move the cursor characters to the right and
  WiFi.begin(ssid, pass);
  Blynk.config(auth);  // in place of Blynk.begin(auth, ssid, pass);
  dht.begin();

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

int t1;
int t2;
int t3;
int h1;
int h2;
int h3;
int p1;

void loop() {
  sp();
  lp();
  if (Serial.available() > 0) {
    String input;
    input = Serial.readStringUntil('\n');
    if (input[0] == 'c') {
      WiFi.begin(ssid, pass);
      Serial.println("connecting...");
    }
    else if (input[0] == 'd') {
      WiFi.disconnect();
      Serial.println("disconnecting...");
    }
  }
  if (WiFi.status() == WL_IDLE_STATUS) { // when not connected to a network, but powered on
    // lcd.setCursor(-4, 3);
    // lcd.print("WL_IDLE_STATUS");
    Serial.println("WL_IDLE_STATUS");
  }
  else if (WiFi.status() == WL_CONNECTED) {
    //  lcd.setCursor(-4, 3);
    //  lcd.print(" Wifi_CONNECTED ");
    Serial.print("WL_CONNECTED ");
    Serial.println(WiFi.localIP());
  }
  else if (WiFi.status() == WL_NO_SSID_AVAIL) {
    // lcd.setCursor(-4, 3);
    // lcd.print("WL_NO_SSID_AVAIL");
    Serial.println("WL_NO_SSID_AVAIL");
  }
  else if (WiFi.status() == WL_CONNECT_FAILED) {
    // lcd.setCursor(-4, 3);
    // lcd.print("WL_CONNECT_FAILED");
    Serial.println("WL_CONNECT_FAILED");
  }
  else if (WiFi.status() == WL_CONNECTION_LOST) {
    // lcd.setCursor(-4, 3);
    // lcd.print("CONNECTION--LOST");
    Serial.println("WL_CONNECTION_LOST");
  }
  else if (WiFi.status() == WL_DISCONNECTED) {
    // lcd.setCursor(-4, 3);
    // lcd.print("Wfi_DISCONNECTED");
    Serial.println("WL_DISCONNECTED");
  }
  else {
    digitalWrite(BUILTIN_LED, HIGH);
    Serial.print("unknown status: ");
    Serial.println(WiFi.status());
  }


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

  t1 = bmp180.getTemperature();
  Blynk.virtualWrite(0, t1);

  if (!bmp180.measurePressure())
  {
    Serial.println("could not start perssure measurement, is a measurement already running?");
    return;
  }
  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  p1 = bmp180.getPressure();
  Blynk.virtualWrite(6, p1);

  h1 = dht.readHumidity();
  Blynk.virtualWrite(1, h1);

  Blynk.run();
  BLYNK_WRITE();
}
BLYNK_WRITE(V2) {
  t2 = param.asFloat();
  // Serial.print(String("testing under loop t2: ") + t2);
  Serial.println(" degC");
}
BLYNK_WRITE(V3) {
  h2 = param.asFloat();
}
BLYNK_WRITE(V4) {
  t3 = param.asFloat();
}
BLYNK_WRITE(V5) {
  h3 = param.asFloat();
}

void sp() {
  Serial.print(String("t1: ") + t1);
  Serial.println(" degC");
  Serial.print(String("p1: ") + p1);
  Serial.println(" Pa");
  Serial.print(String("h1: ") + h1);
  Serial.println(" %");
  Serial.print(String("t2: ") + t2);
  Serial.println(" degC");
  Serial.print(String("h2: ") + h2);
  Serial.println(" %");
  Serial.print(String("t3: ") + t3);
  Serial.println(" degC");
  Serial.print(String("h3: ") + h3);
  Serial.println(" %");
}

void lp() {
  lcd.clear();

  lcd.setCursor(4, 0);
  lcd.print("T1:");
  lcd.print(t1);
  lcd.print("*c");

  lcd.setCursor(0, 1);
  lcd.print("H1:");
  lcd.print(h1);
  lcd.print("%");

  lcd.setCursor(10, 1);
  lcd.print("P1:");
  lcd.print(p1 % 100);
  lcd.print("pa");

  delay (3000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("T2:");
  lcd.print(t2);
  lcd.print("*c");

  lcd.setCursor(0, 1);
  lcd.print("H2:");
  lcd.print(h2);
  lcd.print("%");

  lcd.setCursor(10, 0);
  lcd.print("T3:");
  lcd.print(t3);
  lcd.print("*c");

  lcd.setCursor(10, 1);
  lcd.print("H3:");
  lcd.print(h3);
  lcd.print("%");

  delay (3000);
}
