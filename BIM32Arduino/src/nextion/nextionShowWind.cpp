#include "./nextion.hpp"

#include "../validate.hpp"
#include "../languages.hpp"

/**
 * Display wind speed
 */
void Nextion::_showWindSpeed() {
    if(_prevWindSpd != _windSpd or _forced) {
        _nextion.writeStr("Main.wind0.txt", validate.windSpeed(_windSpd) 
            ? (String(int(round(_windSpd))) + lang.ms()) : "--"
        );
        _prevWindSpd = _windSpd;
    }
}

/**
 * Display wind direction
 */
void Nextion::_showWindDirection() {
    if(_prevWindDir != _windDir or _forced) {
        _nextion.writeNum("Main.windDir0.pic", _windDir > 7 ? 42 : _windDir + 42);
        _prevWindDir = _windDir;
    }
}