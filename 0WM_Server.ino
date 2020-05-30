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

#include "Sodaq_DS3231.h"
#include <Wire.h>
#define DISPLAY_DELAY 1000 // oled display delay 1s
#include "SSD1306Wire.h"

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

void loop() {
  {
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
    delay(200);
  }
  Blynk.run();
  BLYNK_WRITE();

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
  Blynk.virtualWrite(0, bmp180.getTemperature());

  lcd.setCursor(0, 0); // Move the cursor characters to the right and
  lcd.print("T1 ");  // Print HELLO to the screen, starting at 0,0.
  lcd.print(bmp180.getTemperature());

 // Blynk.notify("NODE MCU BOARD1 is ON AIR...!");
 // blynk.email("ullas6558@gmail.com", "PROJECT 2020", "HI test code is UP");

  //start a pressure measurement. pressure measurements depend on temperature measurement, you should only start a pressure
  //measurement immediately after a temperature measurement.
  if (!bmp180.measurePressure())
  {
    Serial.println("could not start perssure measurement, is a measurement already running?");
    return;
  }
  do
  {
    delay(100);
  } while (!bmp180.hasValue());

  Serial.print("Pressure: ");
  Serial.print(bmp180.getPressure());
  Serial.println(" Pa");
  Blynk.virtualWrite(6, bmp180.getPressure());

  lcd.setCursor(0, 1); // Move the cursor characters to the right and
  lcd.print("P1 ");  // Print HELLO to the screen, starting at 0,0.
  lcd.print(bmp180.getPressure());
  //lcd.print("pa");

  float uh = dht.readHumidity();
  Serial.print("humidity: ");
  Serial.println(uh);
  Blynk.virtualWrite(1, dht.readHumidity());

  lcd.setCursor(8, 0); // Move the cursor characters to the right and
  lcd.print("H1:");  // Print HELLO to the screen, starting at 0,0.
  lcd.print(dht.readHumidity());
  //lcd.print("%");
  delay(5000);
}
BLYNK_WRITE(V2) {
  float pinData2 = param.asFloat();
  Serial.println(String("temp2: ") + pinData2);
  lcd.setCursor(0, 0); // Move the cursor characters to the right and
  //lcd.clear();
  delay(1000);
  lcd.print("T2 ");  // Print HELLO to the screen, starting at 0,0.
  lcd.print(pinData2);
  //lcd.print("C");
}
BLYNK_WRITE(V3) {
  float pinData3 = param.asFloat();
  Serial.println(String("hum: ") + pinData3);
  lcd.setCursor(0, 1); // Move the cursor characters to the right and
  lcd.print("H2 ");  // Print HELLO to the screen, starting at 0,0.
  lcd.print(pinData3);
  delay(5000);
  //lcd.print("%");
}
/BLYNK_WRITE(V4) {
  float pinData4 = param.asFloat();
  Serial.println(String("temp3: ") + pinData4);
  lcd.setCursor(8, 0); // Move the cursor characters to the right and
  lcd.print("T3:");  // Print HELLO to the screen, starting at 0,0.
  lcd.print(pinData4);
  lcd.print((char)223);
  lcd.print("C");
  }
  BLYNK_WRITE(V5) {
  float pinData5 = param.asFloat();
  Serial.println(String("hum: ") + pinData5);
  lcd.setCursor(8, 1); // Move the cursor characters to the right and
  lcd.print("H3:");  // Print HELLO to the screen, starting at 0,0.
  lcd.print(pinData5);
  //lcd.print("%");
  delay(5000);
  }
