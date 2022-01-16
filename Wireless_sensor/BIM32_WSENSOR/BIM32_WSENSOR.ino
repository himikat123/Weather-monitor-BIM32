/* 
 *  Wireless Sensor BIM v2.0
 *  © himikat123@gmail.com, Nürnberg, Deutschland, 2022
 */

       // Arduino Nano - ATmega328P

/**
 *  Temperature, humidity, pressure sensor
 *  uncomment only one of them
 */
//#define USE_BME280
//#define USE_SHT21
//#define USE_DS18B20

/**
  Ambient light sensor
  uncomment if you need it
*/
//#define USE_MAX44009
/**********************************************************/




#ifdef USE_BME280
  #include <Adafruit_Sensor.h>
  #include <Adafruit_BME280.h>
#endif
#ifdef USE_SHT21
  #include "SHT21.h"
#endif
#ifdef USE_DS18B20
  #include <microDS18B20.h>
#endif
#ifdef USE_MAX44009
  #include <MAX44009.h>
#endif

#define BAT           A2
#define SET            5
#define DONE           3
#define ONE_WIRE       7

struct{
  bool bme280 = false;
  bool sht21 = false;
  bool ds18b20 = false;
  bool max44009 = false;
} detected;

struct{
  float temp = 404.0;
  float hum = 202;
  float pres = 4040;
  float light = -1.0;
  uint16_t adc = 0;
} datas;

#ifdef USE_BME280
  Adafruit_BME280 bme280;
#endif
#ifdef USE_SHT21
  SHT21 SHT21;
#endif
#ifdef USE_DS18B20
  MicroDS18B20<ONE_WIRE> ds18b20;
#endif
#ifdef USE_MAX44009
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

void sensors_init(void){
  //BME280
  #ifdef USE_BME280
    if(bme280.begin(0x77, &Wire)) detected.bme280 = true;
    else if(bme280.begin(0x76, &Wire)) detected.bme280 = true;
  #endif

  //SHT21
  #ifdef USE_SHT21
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
  #endif

  //DS18B20
  #ifdef USE_DS18B20
    ds18b20.setResolution(10);
    ds18b20.requestTemp();
  #endif
  
  //MAX44009
  #ifdef USE_MAX44009
    if(!max44009.begin()) detected.max44009 = true;
  #endif
}

void get_datas(void){
  //BME280
  #ifdef USE_BME280
    if(detected.bme280){
      datas.temp = bme280.readTemperature();
      datas.hum = bme280.readHumidity();
      datas.pres = bme280.readPressure() / 100.0F;
    }
  #endif

  //SHT21
  #ifdef USE_SHT21
    if(detected.sht21){
      datas.temp = SHT21.getTemperature();
      datas.hum = SHT21.getHumidity();
    }
  #endif

  //DS18B20
  #ifdef USE_DS18B20
    delay(200);
    if(ds18b20.readTemp()) detected.ds18b20 = true;
    if(detected.ds18b20) datas.temp = ds18b20.getTemp();
  #endif
  
  //MAX44009
  #ifdef USE_MAX44009
    if(detected.max44009) datas.light = max44009.get_lux();
  #endif
    
  //Battery
  datas.adc = analogRead(BAT);
}

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
