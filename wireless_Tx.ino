/*
 Tx, transmit MyData temp and hum from BMP280
 by nRF2401
 CE= 7; SCK= 13; MISO= 12; CSN= 8; MOSI= 11;
*/
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
const float tp_adj = 0.5; // adjust temperature

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

struct MyData {
  float h;  float t;
};
MyData data;

const int delayTime = 10000; // transmit every delay
Adafruit_BME280 bme; // I2C
float temp ;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  bme.begin(0x76);  
}
void loop() {
  // const char text[] = "Bonjour Samuel";
  // radio.write(&text, sizeof(text));
  temp = (bme.readTemperature() -tp_adj);
  data.h = (bme.readHumidity());
  data.t = (bme.readTemperature() - tp_adj);
  radio.write(&data, sizeof(MyData));
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.println();
  delay(delayTime);
}
