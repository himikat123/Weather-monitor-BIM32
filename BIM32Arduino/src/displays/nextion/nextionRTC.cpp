#include "./nextion.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../../config/config.hpp"

/**
 * Set the time and date of the display with built-in RTC
 */
void Nextion::setDisplayRTC() {
    if(
        config.display.model(DISPLAY_1) == D_NX4832K035 or 
        config.display.model(DISPLAY_1) == D_NX4827K043
    ) _setRTC();
}

/**
 * Set the time and date of the NX4832K035 display
 */
void Nextion::_setRTC() {
    if(now() > 1700000000) {
        _nextion.writeNum("rtc5", second());
        _nextion.writeNum("rtc4", minute());
        _nextion.writeNum("rtc3", hour());
        _nextion.writeNum("rtc2", day());
        _nextion.writeNum("rtc1", month());
        _nextion.writeNum("rtc0", year());
    }
}