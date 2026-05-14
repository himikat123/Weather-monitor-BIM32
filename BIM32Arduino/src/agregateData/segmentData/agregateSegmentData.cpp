#include <Arduino.h>
#include "./agregateSegmentData.hpp"

#include "../../globals.hpp"
#include "../../config.hpp"
#include "../../sensors.hpp"
#include "../../weather.hpp"
#include "../../thingspeak.hpp"
#include "../../wirelessSensor.hpp"

float AgregateSegmentData::slotData(uint8_t sensor, uint8_t type, uint8_t timeSlot, uint8_t dispNum, uint8_t* dType) {
    float data = UNDEFINED_FLOAT;

    switch(sensor) {
        case CLOCK_SOURCE: { // Clock
            *dType = CLOCK;
        }; break;
        case DATE_SOURCE: { // Date
            *dType = DATE_TYPE;
        }; break;
        case BME280_SOURCE: { // BME280
            if(type == 0) {
                data = sensors.get_bme280_temp();
                *dType = TEMP_TYPE;
            }
            if(type == 1) {
                data = sensors.get_bme280_hum();
                *dType = HUM_TYPE;
            }
            if(type == 2) {
                data = sensors.get_bme280_pres();
                *dType = PRES_TYPE;
            }
        }; break;
        case BMP180_SOURCE: {// BMP180
            if(type == 0) {
                data = sensors.get_bmp180_temp();
                *dType = TEMP_TYPE;
            }
            if(type == 1) {
                data = sensors.get_bmp180_pres();
                *dType = PRES_TYPE;
            }
        }; break;
        case SHT21_SOURCE: {// SHT21
            if(type == 0) {
                data = sensors.get_sht21_temp();
                *dType = TEMP_TYPE;
            }
            if(type == 1) {
                data = sensors.get_sht21_hum();
                *dType = HUM_TYPE;
            }
        }; break;
        case DHT22_SOURCE: { // DHT22
            if(type == 0) {
                data = sensors.get_dht22_temp();
                *dType = TEMP_TYPE;
            }
            if(type == 1) {
                data = sensors.get_dht22_hum();
                *dType = HUM_TYPE;
            }
        }; break;
        case DS18B20_SOURCE: { // DS18B20
            data = sensors.get_ds18b20_temp();
            *dType = TEMP_TYPE;
        }; break;
        case ESP32_SOURCE: { // ESP32
            data = sensors.get_esp32_temp();
            *dType = TEMP_TYPE;
        }; break;
        case THING_SOURCE: { // Thingspeak
            data = thingspeak.dataRelevance()
                ? thingspeak.get_field(config.display.timeSlot.thing(timeSlot, dispNum))
                : 40400.0;
            if(type == 0) *dType = TEMP_TYPE;
            if(type == 1) *dType = HUM_TYPE;
            if(type == 2) *dType = PRES_TYPE;
        }; break;
        case WEATHER_SOURCE: { // Weather
            bool dataRelevant = (now() - weather.get_currentUpdated() < 1200);
            if(type == 0) {
                data = dataRelevant ? weather.get_currentTemp() : 40400.0;
                *dType = TEMP_TYPE;
            }
            if(type == 1) {
                data = dataRelevant ? weather.get_currentHum() : 40400.0;
                *dType = HUM_TYPE;
            }
            if(type == 2) {
                data = dataRelevant ? weather.get_currentPres() : 40400.0;
                *dType = PRES_TYPE;
            }
        }; break;
        case WSENSOR_SOURCE: { // Wireless sensor
            unsigned int wsensNum = config.display.timeSlot.wsensor.num(timeSlot, dispNum);
            unsigned int wsensType = config.display.timeSlot.wsensor.type(timeSlot, dispNum);
            if(wsensType <= 4) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_temperature(wsensNum, wsensType)
                    : 40400.0;
                *dType = TEMP_TYPE;
            }
            if(wsensType == 5) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_humidity(wsensNum)
                    : 40400.0;
                *dType = HUM_TYPE;
            }
            if(wsensType == 6) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_pressure(wsensNum)
                    : 40400.0;
                *dType = PRES_TYPE;
            }
            if(wsensType == 7) {
                data = wsensor.dataRelevance(wsensNum)
                    ? wsensor.get_co2(wsensNum)
                    : 40400.0;
                *dType = CO2_TYPE;
            }
        }; break;
        case BME680_SOURCE: { // BME680
            if(type == 0) {
                data = sensors.get_bme680_temp();
                *dType = TEMP_TYPE;
            }
            if(type == 1) {
                data = sensors.get_bme680_hum();
                *dType = HUM_TYPE;
            }
            if(type == 2) {
                data = sensors.get_bme680_pres();
                *dType = PRES_TYPE;
            }
            if(type == 3) {
                data = sensors.get_bme680_iaq();
                *dType = IAQ_TYPE;
            }
        }; break;
        default: ; break;
    }

    return data;
}