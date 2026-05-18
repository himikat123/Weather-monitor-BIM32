#include "./ili9341.hpp"

#include "../../validation/validate.hpp"
#include "./pictures/bats/bat1.hpp"
#include "./pictures/bats/bat2.hpp"
#include "./pictures/bats/bat3.hpp"
#include "./pictures/bats/bat4.hpp"

/**
 * Display battery symbol
 */
void ILI9341::_showBatteryLevel() {
    if(_prevBatLevel != _batLevel || _forced) {
        if(validate.batLvl(_batLevel)) {
            switch(_batLevel) {
                case 1: _showImg(258, 2, bat1, sizeof(bat1)); break;
                case 2: _showImg(258, 2, bat2, sizeof(bat2)); break;
                case 3: _showImg(258, 2, bat3, sizeof(bat3)); break;
                case 4: _showImg(258, 2, bat4, sizeof(bat4)); break;
                default: tft.fillRect(258, 2, 32, 21, BG_COLOR); break;
            }
        }
        else tft.fillRect(258, 2, 32, 21, BG_COLOR);
        _prevBatLevel = _batLevel;
    }
}

/**
 * Display voltage, percentage, CO2 or IAQ
 */
void ILI9341::_showVoltageOrPercentage() {
    if(_prevVolt != _volt || _prevVoltColor != _voltColor || _forced) {
        _printText(178, 10, 78, 16, _volt, FONT1, RIGHT, _air_color[_voltColor]);
        _prevVolt = _volt;
        _prevVoltColor = _voltColor;
    }
}