#include <Arduino.h>
#include <TimeLib.h>
#include "nextion.hpp"

#include "../../weather/weather.hpp"

/**
 * Initialize daily forecast links to hourly forecast
 */
void Nextion::_daily2hourly() {
    unsigned int fd = 2;
    for(uint8_t i=0; i<40; i++) {
        if(hour(weather.get_hourlyDate(i)) == 0) {
            if(i != 0) _nextion.writeNum("Hourly.day" + String(fd++) + ".val", i);
            if(fd > 4) break;
        }
    }
}