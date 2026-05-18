#include "agregateComfortData.hpp"

#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../sensors/sensors.hpp"
#include "../../weather/weather.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"
#include "../../wsensor/wirelessSensor.hpp"

float AgregateComfortData::hum() {
    float hum = UNDEFINED_FLOAT;
    switch(config.comfort.hum.source()) {
        case WEATHER_HUM: 
            hum = weather.get_currentHum(); 
            break;
        case WSENSOR_HUM:
            if(wsensor.dataRelevance(config.comfort.hum.wsensNum()))
                hum = wsensor.get_humidity(config.comfort.hum.wsensNum());
            break;
        case THING_HUM:
            if(thingspeak.dataRelevance())
                hum = thingspeak.get_field(config.comfort.hum.thing());
            break;
        case BME280_HUM: 
            hum = sensors.get_bme280_hum(); 
            break;
        case SHT21_HUM: 
            hum = sensors.get_sht21_hum(); 
            break;
        case DHT22_HUM: 
            hum = sensors.get_dht22_hum(); 
            break;
        case BME680_HUM: 
            hum = sensors.get_bme680_hum(); 
            break;
        default: ; break;
    }
    return hum;
}