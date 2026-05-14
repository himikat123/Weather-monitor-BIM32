#include "./nextion.hpp"

#include "../validate.hpp"

/**
 * Display voltage, percentage, CO2 or IAQ
 */
void Nextion::_showVoltage() {
    if(_prevVolt != _volt or _forced) {
        _nextion.writeStr("Main.uBat.txt", _volt);
        _prevVolt = _volt;
    } 
    if(_prevVoltColor != _voltColor or _forced) {
        if(_voltColor > 3) _voltColor = 0;
        _nextion.writeNum("Main.uBat.pco", _air_color[_voltColor]);
        _prevVoltColor = _voltColor;
    }
}

/**
 * Display battery symbol
 */
void Nextion::_showBattery() {
    if(_prevBatLevel != _batLevel or _forced) {
        _nextion.writeNum("Main.bat.pic", validate.batLvl(_batLevel) ? (_batLevel + 35) : 35);
        _prevBatLevel = _batLevel;
    }
}