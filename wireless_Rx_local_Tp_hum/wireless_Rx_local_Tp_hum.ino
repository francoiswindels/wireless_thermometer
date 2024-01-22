// Rx
/* receive from NRF2401
 * measure local temperature
 * display remote and local temperature
 * add time?
 * 
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
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
float temp_out_adj = 1.8; // adjustment for outside temperture
float temp_in_adj = 1.0; // adjustment for inside temperture
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
}
void loop() {
  if (radio.available()) {
    //char text[32] = "";
    //radio.read(&text, sizeof(text));
    temp = (bme.readTemperature() -temp_in_adj);
    hum = bme.readHumidity();
    Serial.println("pong ...");
    radio.read(&data, sizeof(MyData));
    Serial.print("Hm out= "); Serial.print(data.h,1); Serial.println(" %");
    Serial.print("Tp out= "); Serial.print(data.t,1); Serial.println(" °C");
    Serial.print("Tp in= "); Serial.print(temp,1); Serial.println(" °C");
    Serial.print("Hm in= "); Serial.print(hum,1); Serial.println(" %");
    // outside temperature and humidity
    lcd.setCursor(0,0); lcd.print("Out");
    lcd.setCursor(4,0); lcd.print(data.t - temp_out_adj,1);
    lcd.setCursor(9,0); lcd.print("C");
    lcd.setCursor(11,0); lcd.print(data.h,1);
    lcd.setCursor(15,0); lcd.print("%");
    // inside temperature and humidity
    lcd.setCursor(0,1); lcd.print("In");
    lcd.setCursor(3,1); lcd.print(temp - temp_out_adj,1 );
    lcd.setCursor(8,1); lcd.print("C");
    lcd.setCursor(11,1); lcd.print(hum,1);
    lcd.setCursor(15,1); lcd.print("%");
  }
  //Serial.println();
  //lcd.setCursor(0,1);
  //lcd.print(temp);
  //delay(delaytime);
}
