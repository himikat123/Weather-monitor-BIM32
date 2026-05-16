#include <Adafruit_BMP085.h> // v1.2.4 https://github.com/adafruit/Adafruit-BMP085-Library

#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Initialize BMP180 sensor
 */
void Sensors::_BMP180Init(void) {
    if(bmp.begin()) _bmp180_det = true;
}

/**
 * Read data from BMP180 sensor
 */
void Sensors::_BMP180Read(void) {
    if(_bmp180_det) {
        state.bmp180.temp = bmp.readTemperature();
        state.bmp180.pres = bmp.readPressure() / 100.0F;
        state.bmp180.updated = true;
    }
    else {
        state.bmp180.temp = 40400.0;
        state.bmp180.pres = 40400.0;
    }
}

float Sensors::get_bmp180_temp() {
    return state.bmp180.temp + config.sensors.bmp180.tempCorr();
}

float Sensors::get_bmp180_pres() {
    float pres = state.bmp180.pres;
    return (config.units_pres() ? pres : mmHg(pres)) + config.sensors.bmp180.presCorr();
}