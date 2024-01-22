// Rx
/* receive from NRF2401
 * measure local temperature
 * display remote and local temperature
 * WORK IN PROGRESS 20230430
 * lcd display libray is broken for 16*4
 * see https://forum.arduino.cc/t/solved-16x4-lcd-characters-in-row-3-4-are-moved-to-the-right/62808
 */
#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C
float temp ;
float baro;
float hum;
unsigned long delayTime = 15000;
RF24 radio(9, 10); // CE, CSN
struct MyData {
  float h; float t;
};
MyData data;
LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
float temp_out_adj = 0; // adjustment for outside temperture
float temp_in_adj = 0.5; // adjustment for inside temperture
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  bme.begin(0x76);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  lcd.init();
  lcd.backlight();
  delay(2000);
}
void loop() {
  if (radio.available()) {
    //char text[32] = "";
    //radio.read(&text, sizeof(text));
    temp = (bme.readTemperature() -temp_in_adj);
    hum = bme.readHumidity();
    Serial.println("pong ...");
    radio.read(&data, sizeof(MyData));
    Serial.print("Hm out= "); Serial.print(data.h); Serial.println(" %");
    Serial.print("Tp out= "); Serial.print(data.t); Serial.println(" °C");
    Serial.print("Tp in= "); Serial.print(temp); Serial.println(" °C");
    Serial.print("Hm in= "); Serial.print(hum); Serial.println(" %");
    lcd.clear();
    lcd.setCursor(0,0); lcd.print("Out:");
    lcd.setCursor(5,0); lcd.print(data.t - temp_out_adj,1);
    lcd.setCursor(10,0); lcd.print("C");
    lcd.setCursor(0,1); lcd.print("Hum:");
    lcd.setCursor(5,1); lcd.print(data.h,1);
    lcd.setCursor(10,1); lcd.print("%");
    // Inside data
    lcd.setCursor(16,0); lcd.print("In:");
    lcd.setCursor(20,0); lcd.print(temp, 1);
    lcd.setCursor(25,0); lcd.print("C");
    lcd.setCursor(16,1); lcd.print("Hum:");
    lcd.setCursor(21,1); lcd.print(hum, 1);
    lcd.setCursor(26,1); lcd.print("%");
    //lcd.setCursor(20,0); lcd.print(temp, 1);
    //lcd.setCursor(25,0); lcd.print("C");
    
  }
  //Serial.println();
  //lcd.setCursor(0,1);
  //lcd.print(temp);
  //delay(delaytime);
}
