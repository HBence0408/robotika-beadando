#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal_I2C.h>
//#include <dht.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <string.h>
#include "DHT.h"


int recv_pin = 7;
IRrecv receiver(recv_pin);

//#define ONE_WIRE_BUS 8
//OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature sensors(&oneWire);
//#define DHT11_PIN 8
//#define DHTTYPE DHT11

//DHT dht11(DHT11_PIN, DHTTYPE );


LiquidCrystal_I2C lcd(0x27,16, 2);

int motor_pin = 9;

float temp;
int limit = 30;
int speed = 0;
int mode = 0;

unsigned long lastTempRequest = 0;
const unsigned long tempInterval = 2000;


void setup() {
  Serial.begin(9600);
  pinMode(recv_pin, INPUT);
  //pinMode(8, INPUT);
  receiver.enableIRIn();
 // receiver.blink13(true);
  pinMode(motor_pin, OUTPUT);
  //dht11.begin();
 // sensors.begin();
  //sensors.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {

   //sensors.requestTemperatures(); 
  
 // Serial.print("Celsius temperature: ");
  //Serial.print(sensors.getTempCByIndex(0)); 
  //Serial.print(" - Fahrenheit temperature: ");
 // Serial.println(sensors.getTempFByIndex(0));




 // if (millis() - lastTempRequest >= tempInterval) {
   // sensors.requestTemperatures();
   // temp = sensors.getTempCByIndex(0);
   // lastTempRequest = millis();
   // lcd.setCursor(0, 0);
   // lcd.print(temp);
   // delay(150);
 // }

/*
// read humidity
 // float humi  = dht11.readHumidity();
  // read temperature as Celsius
//  float tempC = dht11.readTemperature();
  // read temperature as Fahrenheit
  float tempF = dht11.readTemperature(true);
  Serial.print("DHT11# Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
  // check if any reads failed
  if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
    Serial.println("Failed to read from DHT11 sensor!");
  } else {
    Serial.print("DHT11# Humidity: ");
    Serial.print(humi);
    Serial.print("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(tempC);
    Serial.print("°C ~ ");
    Serial.print(tempF);
    Serial.println("°F");
  }

delay(1000);
*/

  switch(mode){ // ki iratas

    case 0:
    lcd.setCursor(0, 1);
    lcd.print("          ");
    break;

    case 1:
     lcd.setCursor(0, 1);
    lcd.print("limit:");
     lcd.setCursor(8, 1);
    lcd.print(limit);
    break;

    case 2:
     lcd.setCursor(0, 1);
    lcd.print("speed:");
     lcd.setCursor(8, 1);
    lcd.print(speed);
    break;

    default:
    break;

  }


   if(receiver.decode()){
    Serial.println(receiver.decodedIRData.decodedRawData);

    String hexString = String(receiver.decodedIRData.decodedRawData, HEX);
    hexString.toUpperCase(); 
    Serial.println(hexString);

    delay(150);

    //lcd.setCursor(0, 1);
    
    //lcd.print(hexString);

    receiver.resume();


    switch(mode){

      case 0:     // mode allitas
        if(hexString == "E916FF00"){
        
        mode = 1;

      }else if(hexString == "E619FF00"){

        mode = 2;

      }

      break;

      case 1:   // limit allitas

      if(hexString == "B946FF00"){
        
        limit++;

      }else if(hexString == "EA15FF00"){

        limit--;

      }else if(hexString == "BF40FF00"){

        mode = 0;

      }

      break;

      case 2:  // sebesseg allitas

      if(hexString == "B946FF00" && speed < 8){

        speed++;

      }else if(hexString == "EA15FF00" && speed > 0){

        speed--;

      }else if(hexString == "BF40FF00"){

        mode = 0;

      }

      break;


      default:
      break;


    }
   }


  // if (temp > limit) {
   int pwmOutput;
   if (speed > 0){
    pwmOutput = (speed + 8) * 5 ;
   }else{
    pwmOutput = 0;
   }

   

 // if (speed == 0)
 // {
   //  pwmOutput = 0; 
     //  }else if (speed == 1)
      // {
       //  pwmOutput = 10; 
      // }else if (speed == 2) 
      // {
      //  pwmOutput = 20; 
     //  }

   analogWrite(motor_pin, pwmOutput);
//}else{
  //analogWrite(motor_pin, 0);
//}
}
