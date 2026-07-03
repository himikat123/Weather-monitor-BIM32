#include <Arduino.h>
#include <TimeLib.h>

#include "./wirelessSensor.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * check if data is not expired
 */
bool WirelessSensor::dataRelevance(uint8_t wsensNum) {
    return (now() - state.wsensor.time[wsensNum]) < (config.wsensor.expire(wsensNum) * 60);
}