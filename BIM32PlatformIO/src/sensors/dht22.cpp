#include "DHTesp.h" // v1.19.0 http://desire.giesecke.tk/index.php/2018/01/30/esp32-dht11/

#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Initialize DHT22 sensor
 */
void Sensors::_DHT22Init(void) {
    dht.setup(DHT22_PIN, DHTesp::DHT22);
    dht.getTempAndHumidity();
    dht.getStatus();
    if(dht.getStatus() == 0) _dht22_det = true;
}

/**
 * Read data from DHT22 sensor
 */
void Sensors::_DHT22Read(void) {
    if(_dht22_det) {
        state.dht22.temp = dht.getTemperature();
        state.dht22.hum = dht.getHumidity();
        state.dht22.updated = true;
    }
    else {
        state.dht22.temp = 40400.0;
        state.dht22.hum = 40400.0;
    }
}

float Sensors::get_dht22_temp() {
    return state.dht22.temp + config.sensors.dht22.tempCorr();
}

float Sensors::get_dht22_hum() {
    return state.dht22.hum + config.sensors.dht22.humCorr();
}