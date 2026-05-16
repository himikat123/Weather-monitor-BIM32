#include <MAX44009.h> // v1.2.3 https://github.com/dantudose/MAX44009

#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Initialize MAX44009 sensor
 */
void Sensors::_MAX44009Init(void) {
    if(!max_light.begin()) _max44009_det = true;
}

/**
 * Read data from MAX44009 sensor
 */
void Sensors::_MAX44009Read(void) {
    if(_max44009_det) {
        state.max44009.light = max_light.get_lux();
        state.max44009.updated = true;
    }
    else state.max44009.light = -10000.0;
}

float Sensors::get_max44009_light() {
    return state.max44009.light + config.sensors.max44009.lightCorr();
}