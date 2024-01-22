// Rx

#include <SPI.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
RF24 radio(9, 10); // CE, CSN
struct MyData {
  float h; float t;
};
MyData data;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
float temp = 20.5;
float temp_adj = 1.8; // adjustment for temperture
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
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
    Serial.println("pong ...");
    radio.read(&data, sizeof(MyData));
    Serial.print("H = "); Serial.print(data.h); Serial.println(" %");
    Serial.print("T = "); Serial.print(data.t); Serial.println(" °C");
    lcd.setCursor(0,0); lcd.print("Tp out:");
    lcd.setCursor(7,0); lcd.print(data.t - temp_adj);
    lcd.setCursor(12,0); lcd.print("C");
    lcd.setCursor(0,1); lcd.print("Hm out:");
    lcd.setCursor(7,1); lcd.print(data.h);
    lcd.setCursor(12,1); lcd.print("%");
  }
  //Serial.println();
  //lcd.setCursor(0,1);
  //lcd.print(temp);
  delay(5000);
}
