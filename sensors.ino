#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP085.h>
#include "SHT21.h"
#include "DHTesp.h"
#include <MAX44009.h>
#include <BH1750.h>

int dhtPin = 4;

SHT21 SHT21;
DHTesp dht;
MAX44009 max_light;
BH1750 lightMeter(0x23);
Adafruit_BMP085 bmp;
Adafruit_BME280 bme;
Adafruit_Sensor *bme_temp = bme.getTemperatureSensor();
Adafruit_Sensor *bme_pressure = bme.getPressureSensor();
Adafruit_Sensor *bme_humidity = bme.getHumiditySensor();


void sensors_init(void){
  Wire.begin();
    //BME280
  sensors.bme280_det = bme.begin(0x76, &Wire);
  if(!sensors.bme280_det) sensors.bme280_det = bme.begin(0x77, &Wire);
    //BMP180
  if(bmp.begin()) sensors.bmp180_det = true;
    //SHT21;
  SHT21.begin();
  Wire.beginTransmission(SHT21_ADDRESS);
  Wire.write(0xE7);
  Wire.endTransmission();
  vTaskDelay(100);
  Wire.requestFrom(SHT21_ADDRESS, 1);
  if(Wire.available() == 1){
    Wire.read();
    sensors.sht21_det = true;
  }
    //DHT22
  dht.setup(dhtPin, DHTesp::DHT22);
  dht.getTempAndHumidity();
  dht.getStatus();
  if(dht.getStatus() == 0) sensors.dht22_det = true;
    //MAX44009
  if(!max_light.begin()){
    sensors.max44009_det = true;
  }
    //BH1750
  if(lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2)){
    sensors.bh1750_det = true;
  }
 
  Serial.print("sensors.bme280_det"); Serial.println(sensors.bme280_det);
  Serial.print("sensors.bmp180_det"); Serial.println(sensors.bmp180_det);
  Serial.print("sensors.sht21_det"); Serial.println(sensors.sht21_det);
  Serial.print("sensors.dht22_det"); Serial.println(sensors.dht22_det);
  Serial.print("sensors.max44009_det"); Serial.println(sensors.max44009_det);
  Serial.print("sensors.bh1750_det"); Serial.println(sensors.bh1750_det);
}

void sensors_read(void){
/////////// Temperature //////////////////////////////////
  if(sensors.bme280_det){
    sensors_event_t temp_event;
    bme_temp -> getEvent(&temp_event);
    sensors.bme280_temp = temp_event.temperature + sensors.bme280_temp_corr;
  }
  if(sensors.bmp180_det) sensors.bmp180_temp = bmp.readTemperature() + sensors.bmp180_temp_corr;
  if(sensors.sht21_det) sensors.sht21_temp = SHT21.getTemperature() + sensors.sht21_temp_corr;
  if(sensors.dht22_det) sensors.dht22_temp = dht.getTemperature() + sensors.dht22_temp_corr;
/////////// Humidity //////////////////////////////////
  if(sensors.bme280_det){
    sensors_event_t humidity_event;
    bme_humidity -> getEvent(&humidity_event);
    sensors.bme280_hum = humidity_event.relative_humidity + sensors.bme280_hum_corr;
  }
  if(sensors.sht21_det) sensors.sht21_hum = SHT21.getHumidity() + sensors.sht21_hum_corr;
  if(sensors.dht22_det) sensors.dht22_hum = dht.getHumidity() + sensors.dht22_hum_corr;
////////// Pressure /////////////////////////////////////
  if(sensors.bme280_det){
    sensors_event_t pressure_event;
    bme_pressure -> getEvent(&pressure_event);
    sensors.bme280_pres = pressure_event.pressure + sensors.bme280_pres_corr;
  }
  if(sensors.bmp180_det) sensors.bmp180_pres = bmp.readPressure() / 100 + sensors.bmp180_pres_corr;
  sensors.bmp180_pres *= 0.75;
////////// Ambient light ///////////////////////////////
  if(sensors.max44009_det) sensors.max44009_light = max_light.get_lux() + sensors.max44009_light_corr;
  if(sensors.bh1750_det) sensors.bh1750_light = lightMeter.readLightLevel() + sensors.bh1750_light_corr;
}
