#include "./nextion.hpp"

#include "../../validation/validate.hpp"
#include "../../config/config.hpp"

/**
 * Display temperature inside
 */
void Nextion::_showTempIn() {
    if(_prevTempIn != _tempIn or _forced) {
        if(config.display.source.tempIn.sens() != 4) {
            String buf = validate.temp(_tempIn) ? String((int)round(_tempIn)) : "--";
            buf += "°C";
            _nextion.writeStr("Main.tempInside.txt", buf);
        }
        _prevTempIn = _tempIn;
    }
}

/**
 * Display temperature outside
 */
void Nextion::_showTempOut() {
    if(_prevTempOut != _tempOut or _forced) {
        _showThermometer();
        String buf = validate.temp(_tempOut) ? String((int)round(_tempOut)) : "--";
        buf += "°C";
        _nextion.writeStr("Main.tempOutside.txt", buf);
        _prevTempOut = _tempOut;
    }
}

/**
 * Display thermometer icon (red or blue)
 */
void Nextion::_showThermometer() {
    _nextion.writeNum("Main.thermometer.pic", _tempOut < 0.0 ? 40 : 41);
}