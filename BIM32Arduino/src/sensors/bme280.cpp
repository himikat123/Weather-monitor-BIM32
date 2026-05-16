#include <Adafruit_BME280.h> // v2.2.4 https://github.com/adafruit/Adafruit_BME280_Library

#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Initialize BME280 sensor
 */
void Sensors::_BME280Init(void) {
    _bme280_det = bme.begin(0x76, &Wire);
    if(!_bme280_det) _bme280_det = bme.begin(0x77, &Wire);
}

/**
 * Read data from BME280 sensor
 */
void Sensors::_BME280Read(void) {
    if(_bme280_det) {
        state.bme280.temp = bme.readTemperature();
        state.bme280.hum = bme.readHumidity();
        state.bme280.pres = bme.readPressure() / 100.0F;
        state.bme280.updated = true;
    }
    else {
        state.bme280.temp = 40400.0;
        state.bme280.hum = 40400.0;
        state.bme280.pres = 40400.0;
    }
}

float Sensors::get_bme280_temp() {
    return state.bme280.temp + config.sensors.bme280.tempCorr();
}

float Sensors::get_bme280_hum() {
    return state.bme280.hum + config.sensors.bme280.humCorr();
}

float Sensors::get_bme280_pres() {
    float pres = state.bme280.pres;
    return (config.units_pres() ? pres : mmHg(pres)) + config.sensors.bme280.presCorr();
}