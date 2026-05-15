#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "./nextion.hpp"

#include "../config.hpp"


/**
 * Display the time and date of the last weather update
 */
void Nextion::_showUpdated() {
    if(_prevWeatherUpdated != _weatherUpdated or _forced) {
        time_t t = _weatherUpdated;
        char buf[32] = "";
        unsigned int hr = config.clock.format() > 1 ? hour(t) : hourFormat12(t);
        if(config.clock.format() % 2 == 0) sprintf(buf, "⭮ %02d.%02d.%d %d:%02d:%02d", day(t), month(t), year(t), hr, minute(t), second(t)); 
        else sprintf(buf, "⭮ %02d.%02d.%d %02d:%02d:%02d", day(t), month(t), year(t), hr, minute(t), second(t));
        _nextion.writeStr("Main.updatedTime.txt", t > 0 ? buf : "--");
        _prevWeatherUpdated = _weatherUpdated;
    }
}