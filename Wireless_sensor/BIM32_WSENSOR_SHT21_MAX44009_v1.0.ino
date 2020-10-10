/* 
 *  Wireless Sensor BIM (SHT21 & MAX44009) v1.0
 *  © himikat123@gmail.com, Nürnberg, Deutschland, 2020
 */

       // Arduino Nano - ATmega328P

#include "SHT21.h"
#include <MAX44009.h>
#include <EEPROM.h>

#define BAT           A2
#define SET            5
#define DONE           3

struct{
  bool sht21 = false;
  bool max44009 = false;
} detected;

struct{
  float temp = 404.0;
  float hum = 202;
  float light = -1.0;
  uint16_t adc = 0;
} datas;

SHT21 SHT21;
MAX44009 max_light;

void setup(){
  digitalWrite(DONE, LOW);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(SET, OUTPUT);
  digitalWrite(SET, HIGH);
  pinMode(DONE, OUTPUT);
  digitalWrite(DONE, LOW);
  analogReference(INTERNAL);
  Serial.begin(9600);
  while(!Serial);
  if(EEPROM.read(1)){
    Serial.println("EEPROM.read(1)");
    uint8_t channel = EEPROM.read(0);
    if(channel <= 100){
      Serial.println("channel <= 100");
      digitalWrite(SET, LOW);
      delay(45);
      String ch = "AT+C";
      ch += channel < 100 ? "0" : "";
      ch += channel < 10 ? "0" : "";
      ch += String(channel);
      Serial.println(ch);
      delay(50);
      digitalWrite(SET, HIGH);
      EEPROM.write(1, 0);
    }
  }
  sensors_init();
  dataSend();
  delay(30);
}

void loop(){
  digitalWrite(DONE, HIGH);
  delay(5);
  digitalWrite(DONE, LOW);
  delay(5);
  digitalWrite(DONE, HIGH);
  delay(10);
  while(1){
    delay(5000);
    sensors_init();
    dataSend();
    digitalWrite(DONE, HIGH);
    delay(10);
    digitalWrite(DONE, LOW);
    delay(10);
    digitalWrite(DONE, HIGH);
  }
}

void sensors_init(void){
    //SHT21;
  SHT21.begin();
  Wire.beginTransmission(SHT21_ADDRESS);
  Wire.write(0xE7);
  Wire.endTransmission();
  delay(50);
  Wire.requestFrom(SHT21_ADDRESS, 1);
  if(Wire.available() == 1){
    Wire.read();
    detected.sht21 = true;
  }
    //MAX44009
  if(!max_light.begin()) detected.max44009 = true;
}

void get_datas(void){
  if(detected.sht21){
    datas.temp = SHT21.getTemperature();
    datas.hum = SHT21.getHumidity();
  }
  if(detected.max44009) datas.light = max_light.get_lux();
  datas.adc = analogRead(BAT);
}

void dataSend(void){
  get_datas();
  Serial.print("{\"t\":["); Serial.print(datas.temp);
  Serial.print(",0],\"h\":["); Serial.print(datas.hum);
  Serial.print(",0],\"l\":["); Serial.print(datas.light);
  Serial.print(",1],\"b\":"); Serial.print(datas.adc);
  Serial.print(",\"s\":[");
  if(detected.sht21) Serial.print("\"SHT21\"");
  if(detected.sht21 and detected.max44009) Serial.print(",");
  if(detected.max44009) Serial.print("\"MAX44009\"");
  Serial.println("]}");
  Serial.flush();
}
