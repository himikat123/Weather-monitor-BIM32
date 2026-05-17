#include <Arduino.h>
#include "./thingspeak.hpp"

#include "../../config/config.hpp"
#include "../../validation/validate.hpp"
#include "../../sensors/sensors.hpp"
#include "../../weather/weather.hpp"
#include "../../wsensor/wirelessSensor.hpp"

/**
 * Prepare weather history data for the desired field according to the settings
 * @param field number
 * @return a string with field name and data
 */
String Thingspeak::_historyFieldPrepare(unsigned int fieldNum) {
    String fields = "";
    String field = "&field" + String(fieldNum + 1) + "=";

    if(config.cloud.history.fields(fieldNum) == 1 and (fieldNum == 0 or fieldNum == 3)) { // Weather Temperature
        if(validate.temp(weather.get_currentTemp()))
            fields = field + String(weather.get_currentTemp());
    }
    if(config.cloud.history.fields(fieldNum) == 1 and (fieldNum == 1 or fieldNum == 4)) { // Weather Humidity
        if(validate.hum(weather.get_currentHum()))
            fields = field + String(weather.get_currentHum());
    }
    if(config.cloud.history.fields(fieldNum) == 1 and fieldNum == 2) {                    // Weather Pressure
        if(validate.pres(weather.get_currentPres())) 
            fields = field + String(weather.get_currentPres());
    }
                                                                                    // Wireless sensor
    if((config.cloud.history.fields(fieldNum) == 2 and fieldNum <= 4) or (config.cloud.history.fields(fieldNum) == 1 and fieldNum == 6)) {
        unsigned int wsensNum = config.cloud.history.wsensors(fieldNum);
        unsigned int wsensType = config.cloud.history.wtypes(fieldNum);
        if(wsensor.dataRelevance(wsensNum)) {
            float temp = wsensor.get_temperature(wsensNum, wsensType);
            float hum = wsensor.get_humidity(wsensNum);
            float pres = wsensor.get_pressure(wsensNum);
            float co2 = wsensor.get_co2(wsensNum);
            if((fieldNum == 0 or fieldNum == 3) and validate.temp(temp)) fields = field + String(temp);
            if((fieldNum == 1 or fieldNum == 4) and validate.hum(hum)) fields = field + String(hum);
            if(fieldNum == 2 and validate.pres(pres)) fields = field + String(pres);
            if(fieldNum == 6 and validate.co2(co2)) fields = field + String(co2);
        }
    }

    if(config.cloud.history.fields(fieldNum) == 3) {                                      // Thingspeak
        float value = get_field(config.cloud.history.types(fieldNum));
        if(value >= -50.0 and value <= 1200.0) fields = field + String(value);
    }

    if(config.cloud.history.fields(fieldNum) == 4 and (fieldNum == 0 or fieldNum == 3)) { // BME280 Temperature
        if(validate.temp(sensors.get_bme280_temp())) 
            fields = field + String(sensors.get_bme280_temp());
    }

    if(config.cloud.history.fields(fieldNum) == 4 and (fieldNum == 1 or fieldNum == 4)) { // BME280 Humidity 
        if(validate.hum(sensors.get_bme280_hum()))
            fields = field + String(sensors.get_bme280_hum());
    }

    if(config.cloud.history.fields(fieldNum) == 4 and fieldNum == 2) {                    // BME280 Pressure
        if(validate.pres(sensors.get_bme280_pres()))
            fields = field + String(sensors.get_bme280_pres());
    }

    if(config.cloud.history.fields(fieldNum) == 5 and (fieldNum == 0 or fieldNum == 3)) { // BMP180 Temperature
        if(validate.temp(sensors.get_bmp180_temp())) 
            fields = field + String(sensors.get_bmp180_temp());
    }

    if(config.cloud.history.fields(fieldNum) == 5 and fieldNum == 2) {                    // BMP180 Pressure
        if(validate.pres(sensors.get_bmp180_pres())) 
            fields = field + String(sensors.get_bmp180_pres());
    }

    if(config.cloud.history.fields(fieldNum) == 5 and (fieldNum == 1 or fieldNum == 4)) { // SHT21 Humidity
        if(validate.hum(sensors.get_sht21_hum()))
            fields = field + String(sensors.get_sht21_hum());
    }

    if(config.cloud.history.fields(fieldNum) == 6 and (fieldNum == 0 or fieldNum == 3)) { // SHT21 Temperature
        if(validate.temp(sensors.get_sht21_temp())) 
            fields = field + String(sensors.get_sht21_temp());
    }

    if(config.cloud.history.fields(fieldNum) == 6 and (fieldNum == 1 or fieldNum == 4)) { // DHT22 Humidity
        if(validate.hum(sensors.get_dht22_hum())) 
            fields = field + String(sensors.get_dht22_hum());
    }

    if(config.cloud.history.fields(fieldNum) == 7 and (fieldNum == 0 or fieldNum == 3)) { // DHT22 Temperature
        if(validate.temp(sensors.get_dht22_temp())) 
            fields = field + String(sensors.get_dht22_temp());
    }
  
    if(config.cloud.history.fields(fieldNum) == 8 and (fieldNum == 0 or fieldNum == 3)) { // DS18B20 Temperature
        if(validate.temp(sensors.get_ds18b20_temp()))
            fields = field + String(sensors.get_ds18b20_temp());
    }

    if(config.cloud.history.fields(fieldNum) == 9 and (fieldNum == 0 or fieldNum == 3)) { // BME680 Temperature
        if(validate.temp(sensors.get_bme680_temp())) 
            fields = field + String(sensors.get_bme680_temp());
    }

    if(config.cloud.history.fields(fieldNum) == 7 and (fieldNum == 1 or fieldNum == 4)) { // BME680 Humidity 
        if(validate.hum(sensors.get_bme680_hum()))
            fields = field + String(sensors.get_bme680_hum());
    }

    if(config.cloud.history.fields(fieldNum) == 6 and fieldNum == 2) {                    // BME680 Pressure
        if(validate.pres(sensors.get_bme680_pres()))
            fields = field + String(sensors.get_bme680_pres());
    }

    if(config.cloud.history.fields(fieldNum) == 1 and fieldNum == 5) {                    // BME680 IAQ
        if(validate.iaq(sensors.get_bme680_iaq())) 
            fields = field + String(sensors.get_bme680_iaq());
    }
 
    return fields;
}