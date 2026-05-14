#include "agregateComfortData.hpp"

#include "../../globals.hpp"
#include "../../config.hpp"
#include "../../sensors.hpp"
#include "../../weather.hpp"
#include "../../thingspeak.hpp"
#include "../../wirelessSensor.hpp"

float AgregateComfortData::temp() {
    float temp = UNDEFINED_FLOAT;
    switch(config.comfort.temp.source()) {
        case WEATHER_TEMP: 
            temp = weather.get_currentTemp(); 
            break;
        case WSENSOR_TEMP:
            if(wsensor.dataRelevance(config.comfort.temp.wsensNum()))
                temp = wsensor.get_temperature(config.comfort.temp.wsensNum(), config.comfort.temp.sens());
            break;
        case THING_TEMP:
            if(thingspeak.dataRelevance()) 
                temp = thingspeak.get_field(config.comfort.temp.thing());
            break;
        case BME280_TEMP: 
            temp = sensors.get_bme280_temp(); 
            break;
        case BMP180_TEMP: 
            temp = sensors.get_bmp180_temp(); 
            break;
        case SHT21_TEMP: 
            temp = sensors.get_sht21_temp(); 
            break;
        case DHT22_TEMP: 
            temp = sensors.get_dht22_temp(); 
            break;
        case DS18B20_TEMP: 
            temp = sensors.get_ds18b20_temp(); 
            break;
        case BME680_TEMP: 
            temp = sensors.get_bme680_temp(); 
            break;
        default: ; break;
    }
    return temp;
}