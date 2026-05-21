#include <Arduino.h>
#include "./taskDisplay.hpp"
#include "../state/state.hpp"

void TaskDisplay::_brightnessAndDotFreq() {
    
    /* 7 segment display slow down points blinking frequency if the device isn't connected to the network */
    uint16_t dotFreq = state.net_connected ? 500 : 1000;
    _display->setDotFreq(dotFreq);

    /* brightness change */
    _display->brightness(_get_brightness(), state.reduc[_dispNum]);
}