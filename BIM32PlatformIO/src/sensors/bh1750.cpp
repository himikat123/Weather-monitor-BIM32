#include <BH1750.h> // v1.3.0 https://github.com/claws/BH1750

#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Initialize BH1750 sensor
 */
void Sensors::_BH1750Init(void) {
    if(lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE_2)) _bh1750_det = true;
}

/**
 * Read data from BH1750 sensor
 */
void Sensors::_BH1750Read(void) {
    if(_bh1750_det) {
        state.bh1750.light = lightMeter.readLightLevel();
        state.bh1750.updated = true;
    }
    else state.bh1750.light = -10000.0;
}

float Sensors::get_bh1750_light() {
    return state.bh1750.light + config.sensors.bh1750.lightCorr();
}