#include <Arduino.h>
#include <TimeLib.h>

#include "./taskDisplay.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

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

void TaskDisplay::_itsOffTime() {
    /* Check if need and it's time to turn off the display */
    if(_isTimeoutOffTime()) {
        if(config.display.type(DISPLAY_2) == PIXEL_LEDS_DISPLAY) { 
            if(_display->isdisplayOn()) _display->displayOff();
        }
    }

    uint8_t itsOffTime = _isNightOffTime() ? 1 : 0;
    if((state.disp_night_state[_dispNum] != itsOffTime) || (state.display_state[_dispNum] > 0)) {
        state.disp_night_state[_dispNum] = itsOffTime;
        if(state.display_state[_dispNum] > 0) {
            itsOffTime = state.display_state[_dispNum] - 1;
            state.display_state[_dispNum] = 0;
        }
        if(itsOffTime) {
            if(_display->isdisplayOn()) _display->displayOff();
        }
        else {
            if(!_display->isdisplayOn()) _display->displayOn(false);
        }
    }
}