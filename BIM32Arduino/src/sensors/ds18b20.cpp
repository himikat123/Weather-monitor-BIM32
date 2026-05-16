#include <OneWire.h> // v2.3.7 https://github.com/PaulStoffregen/OneWire
#include <DallasTemperature.h> // v3.9.0 https://github.com/milesburton/Arduino-Temperature-Control-Library

#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Initialize DS18B20 sensor
 */
void Sensors::_DS18B20Init(void) {
    term.begin();
    _ds18b20_det = term.getDeviceCount();
    if(_ds18b20_det > 0) {
        term.getAddress(thermometer, 0);
        term.setResolution(thermometer, DS18B20_RESOLUTION);
        term.requestTemperatures();
    }
}

/**
 * Read data from DS18B20 sensor
 */
void Sensors::_DS18B20Read(void) {
    if(_ds18b20_det) {
        state.ds18b20.temp = term.getTempC(thermometer);
        state.ds18b20.updated = true;
        term.requestTemperatures();
    }
    else state.ds18b20.temp = 40400.0;
}

float Sensors::get_ds18b20_temp() {
    return state.ds18b20.temp + config.sensors.ds18b20.tempCorr();
}