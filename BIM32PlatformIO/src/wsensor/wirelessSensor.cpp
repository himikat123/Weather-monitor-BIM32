#include <Arduino.h>
#include <TimeLib.h>

#include "./wirelessSensor.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

WirelessSensor wsensor;

void WirelessSensor::handleReceive() {
    while(Serial2.available()) {
        char c = Serial2.read();
        _receivedData[_receivedIndex] = c;
        if(_receivedIndex < 1023) _receivedIndex++;
    }
}

/**
 * check if data is not expired
 */
bool WirelessSensor::dataRelevance(uint8_t wsensNum) {
    return (now() - state.wsensor.time[wsensNum]) < (config.wsensor.expire(wsensNum) * 60);
}