#include <Arduino.h>
#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

#ifdef __cplusplus
  extern "C"{
#endif
  unsigned int temprature_sens_read();
#ifdef __cplusplus
}
#endif

/**
 * Read data from analog ambient light sensor
 */
void Sensors::_AnalogRead(void) {
    float adc = float(analogRead(PHOTORESISTOR_PIN));
    #if defined(BIM32_CYD)
        state.analog.volt = 3.3 - (adc / 1241.0);
    #else
        state.analog.volt = adc / 1241.0;
    #endif
    state.analog.updated = true;
}

/**
 * Read ESP32 temperature
 */
void Sensors::_ESP32Read(void) {
    state.esp32core.temp = (temprature_sens_read() - 32) / 1.8;
    state.esp32core.updated = true;
}

float Sensors::get_esp32_temp() {
    return state.esp32core.temp + config.sensors.esp32.tempCorr();
}

float Sensors::get_analog_voltage() {
    return state.analog.volt + config.sensors.analog.voltageCorr();
}