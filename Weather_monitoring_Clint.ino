//the  code is for clint_1 

#include <Adafruit_Sensor.h>           //In order to use DHT sensor we have to include this library first 
#define BLYNK_PRINT Serial  
//___________________Include Libraries_________________________________________________________
#include <DHT.h>                      //Including the DHT library
#include <ESP8266WiFi.h>              //Including the ESP8266 WiFi library in order to usm them
#include <BlynkSimpleEsp8266.h>       //library for linking up Blynk with ESP8266
#include <Wire.h>                     //For using I2C connection of BMP180 in order to connect it to the board
#include <Adafruit_BMP085.h>          //Including the library for BMP180
Adafruit_BMP085 bmp;                  //Defining the object bmp
#define I2C_SCL 12                    //Connect SCL pin of BMP180 to GPIO12(D6) of Nodemcu
#define I2C_SDA 13                    //Connect SDA pin of BMP180 to GPIO13(D7) of Nodemcu
int audio_signal = A0;
int RS=0; //variable to store RainDrop Sensor Module's output value

float dst,bt,bp,ba;
char dstmp[20],btmp[20],bprs[20],balt[20];
bool bmp085_present=true;

char auth[] = "5KmqoSVW1wPPzukyXAnq45qGBacmUjF9"; //clint_1 ID  //Authentication Key will be there in the Blynk App

bridge1.setAuthToken("6UqgH13ON98rLmxl3b2mO89xjJUoIIG1 "); // server ID
//________________________Mention the SSID and Password____________________________________________________

char ssid[] = "new"; //SSID of the WiFi hotspot available
char pass[] = "12345678";  //Password of the WiFi

#define DHTPIN 2                                    //Connect the DHT11 sensor's data pin to GPIO2(D4) of Nodemcu    
#define DHTTYPE DHT11                               //Mention the type of sensor we are using, Here it it DHT11, for DHT22 just replace DHT11 with DHT22

DHT dht(DHTPIN, DHTTYPE); //Defining the pin and the dhttype

BlynkTimer timer;
void sendSensor()
{
//______________________________Check the working of BMP180 sensor________________________________________
          if (!bmp.begin()) 
          {
              Serial.println("Could not find a valid BMP085 sensor, check wiring!");
              while (1) {}
          }
  
//______________________Getting the Humidity and temperature value from DHT11____________________________
  
          float h = dht.readHumidity();
          
          float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit


//______________________________Check the working of DHT11 sensor________________________________________
  
          if (isnan(h) || isnan(t)) 
          {
              Serial.println("Failed to read from DHT sensor!");
              return;
          }

//______________________________Measuring the Dew Point______________________________________________________

          double gamma = log(h/100) + ((17.62*t) / (243.5+t));
          double dp = 243.5*gamma / (17.62-gamma);
//_____________________________________Checking for audio______________________________________________________

          RS = analogRead(audio_signal);
          RS = constrain(audio_signal, 150, 440); 
          RS = map(audio_signal, 150, 440, 1023, 0);



//______________________Reading the value of Pressure, Temperature, Altitude from the BMP180__________________

  
          float bp =  bmp.readPressure()/100; // Division by 100 makes it in millibars
          
          float ba =  bmp.readAltitude();
          
          float bt =  bmp.readTemperature();
          
          float dst = bmp.readSealevelPressure()/100;

 
 
  
 //_______________Printing the valus of the above read value on to the Virtual Pins in the Bluynk App_____________
 
          Blynk.virtualWrite(V5 , h);
          Blynk.virtualWrite(V6 , t);
          Blynk.virtualWrite(V10, bp);
          Blynk.virtualWrite(V11, ba);
          Blynk.virtualWrite(V12, bt);
          Blynk.virtualWrite(V13, dst);
          Blynk.virtualWrite(V14, dp);
          Blynk.virtualWrite(V15, RS);
          bridge1.virtualWrite(V5 , h);
          bridge1.virtualWrite(V6 , t);
          bridge1.virtualWrite(V10, bp);
          bridge1.virtualWrite(V11, ba);
          bridge1.virtualWrite(V12, bt);
          bridge1.virtualWrite(V13, dst);
          bridge1.virtualWrite(V14, dp);
          bridge1.virtualWrite(V15, RS);   
}


void setup()
{
  
          Serial.begin(9600); //Initializing the Serial Monitor with a Baud Rate of 9600
        
          Blynk.begin(auth, ssid, pass);
        
          dht.begin();  //Initializing the DHT sensor
          
          Wire.begin(I2C_SDA, I2C_SCL); //Initializing the I2C connection
          
          delay(10);
          
          timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
