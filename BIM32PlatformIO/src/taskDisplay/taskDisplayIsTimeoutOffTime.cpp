#include <Arduino.h>
#include <TimeLib.h>

#include "./taskDisplay.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

/**
 * Check if need be and it's time to turn off display
 */
bool TaskDisplay::_isTimeoutOffTime() {
    int off_time = config.display.autoOff(_dispNum) * 60000;
    if(config.display.autoOff(_dispNum) > 0 && ((millis() - state.disp_autoOff[_dispNum]) > off_time)) {
        state.reduc[_dispNum] = true;
        if((millis() - state.disp_autoOff[_dispNum]) > (off_time + 5000)) return true;
    }
    else state.reduc[_dispNum] = false;
    return false;
}