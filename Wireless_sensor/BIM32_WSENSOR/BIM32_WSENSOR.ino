/* 
 *  Wireless Sensor BIM v2.0
 *  © himikat123@gmail.com, Nürnberg, Deutschland, 2022
 */

       // Arduino Nano - ATmega328P

#include "config.h"
#include "BIM32_wsensor.h"

#ifdef USE_BME280
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BME280.h>
  Adafruit_BME280 bme280;
#endif
#ifdef USE_SHT21
  #include "SHT21.h"
  SHT21 SHT21;
#endif
#ifdef USE_DS18B20
  #include <microDS18B20.h>
  MicroDS18B20<ONE_WIRE> ds18b20;
#endif
#ifdef USE_MAX44009
  #include <MAX44009.h>
  MAX44009 max44009;
#endif


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

/**
 * Send data via UART
 */
void dataSend(void){
  get_datas();
  Serial.print("{\"t\":["); Serial.print(datas.temp); Serial.print(",0],");
  Serial.print("\"h\":["); Serial.print(datas.hum); Serial.print(",1],");
  Serial.print("\"p\":["); Serial.print(datas.pres); Serial.print(",2],");
  Serial.print("\"l\":["); Serial.print(datas.light); Serial.print(",3],");
  Serial.print("\"b\":"); Serial.print(datas.adc); Serial.print(",");
  Serial.print("\"s\":[");
  if(detected.bme280) Serial.print("\"BME280\",\"BME280\",\"BME280\",");
  else if(detected.sht21) Serial.print("\"SHT21\",\"SHT21\",\"--\",");
  else if(detected.ds18b20) Serial.print("\"DS18B20\",\"--\",\"--\",");
  else Serial.print("\"--\",\"--\",\"--\",");
  if(detected.max44009) Serial.print("\"MAX44009\"");
  else Serial.print("\"--\"");
  Serial.println("]}");
  Serial.flush();
}
