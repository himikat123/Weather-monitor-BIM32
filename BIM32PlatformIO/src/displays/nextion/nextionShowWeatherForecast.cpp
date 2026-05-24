#include <Arduino.h>
#include "./nextion.hpp"

#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../validation/validate.hpp"
#include "../../languages/languages.hpp"

/**
 * Display daily weather forecast
 */
void Nextion::_showWeatherForecast() {
    for(uint8_t i=0; i<(config.display.model(DISPLAY_1) == D_NX4827K043 ? 5 : 4); i++) {
        if(_prevIcons[i] != _icons[i] or _forced) {
            _nextion.writeNum("Main.icon" + String(i + 1) + ".pic", _icons[i] == 0 ? 29 : (_icons[i] + 20));
            _prevIcons[i] = _icons[i];
        }
        if(_prevDTemps[i] != _dTemps[i] or _forced) {
            String buf = validate.temp(_dTemps[i]) ? String((int)round(_dTemps[i])) : "--";
            buf += "°C";
            _nextion.writeStr("Main.tempMax" + String(i + 1) + ".txt", buf);
            _prevDTemps[i] = _dTemps[i];
        }
        if(_prevNTemps[i] != _nTemps[i] or _forced) {
            String buf = validate.temp(_nTemps[i]) ? String((int)round(_nTemps[i])) : "--";
            buf += "°C";
            _nextion.writeStr("Main.tempMin" + String(i + 1) + ".txt", buf);
            _prevNTemps[i] = _nTemps[i];
        }
        if(_prevWinds[i] != _winds[i] or _forced) {
            _nextion.writeStr("Main.wind" + String(i + 1) + ".txt", validate.windSpeed(_winds[i])
                ? (String(int(round(_winds[i]))) + lang.ms()) : "--"
            );
            _prevWinds[i] = _winds[i];
        }
    }
}