#include <Arduino.h>
#include <TimeLib.h>

#include "./taskDisplay.hpp"
#include "../config/config.hpp"

/**
 * Check if need be and it's time to turn off display
 */
bool TaskDisplay::_isNightOffTime() {
    if(config.display.nightOff.need(_dispNum)) {
        uint16_t fromM = config.display.nightOff.from(_dispNum, false) * 60 + config.display.nightOff.from(_dispNum, true);
        uint16_t toM = config.display.nightOff.to(_dispNum, false) * 60 + config.display.nightOff.to(_dispNum, true);
        uint16_t nowM = hour() * 60 + minute();

        if(fromM < toM) return (fromM <= nowM && nowM < toM);
        else return (nowM >= fromM || nowM < toM);
    }
    return false;
}