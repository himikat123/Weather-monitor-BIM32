#include "SHT21.h" // https://github.com/markbeee/SHT21

#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Initialize SHT21 sensor
 */
void Sensors::_SHT21Init(void) {
    sht21.begin();
    Wire.beginTransmission(SHT21_ADDRESS);
    Wire.write(0xE7);
    Wire.endTransmission();
    delay(100);
    Wire.requestFrom(SHT21_ADDRESS, 1);
    if(Wire.available() == 1) {
        Wire.read();
        _sht21_det = true;
    }
}

/**
 * Read data from SHT21 sensor
 */
void Sensors::_SHT21Read(void) {
    if(_sht21_det) {
        state.sht21.temp = sht21.getTemperature();
        state.sht21.hum = sht21.getHumidity();
        state.sht21.updated = true;
    }
    else {
        state.sht21.temp = 40400.0;
        state.sht21.hum = 40400.0;
    }
}

float Sensors::get_sht21_temp() {
    return state.sht21.temp + config.sensors.sht21.tempCorr();
}

float Sensors::get_sht21_hum() {
    return state.sht21.hum + config.sensors.sht21.humCorr();
}