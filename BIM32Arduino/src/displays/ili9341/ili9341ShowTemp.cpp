#include "./ili9341.hpp"

#include "../../config/config.hpp"
#include "../../validation/validate.hpp"
#include "./pictures/symbols/tempPlus.hpp"
#include "./pictures/symbols/tempMinus.hpp"

/**
 * Display temperature
 */
void ILI9341::_showTemperature(float temp, uint16_t x, uint16_t y, uint8_t font, uint16_t color) {
    String buf = validate.temp(temp) ? String((int)round(temp)) : "--";
    buf += "°C";
    _printText(x, y, font == FONT3 ? 70 : 56, font == FONT3 ? 26 : 20, buf, font, CENTER, color);
}

/**
 * Display temperature inside
 */
void ILI9341::_showTemperatureInside() {
    if(config.display.source.tempIn.sens() == 4) _tempIn = _tempSequence[_sequenceSlot];
    if(_prevTempIn != _tempIn || _forced) {
        _showTemperature(_tempIn, 173, 53, FONT3, TEMPERATURE_COLOR);
        _prevTempIn = _tempIn;
    }
}

/**
 * Display temperature outside
 */
void ILI9341::_showTemperatureOutside() {
    if(_prevTempOut != _tempOut || _forced) {
        _showThermometer();
        _showTemperature(_tempOut, 71, 113, FONT3, TEMPERATURE_COLOR);
        _prevTempOut = _tempOut;
    }
}

/**
 * Display thermometer icon (red or blue)
 */
void ILI9341::_showThermometer() {
    if(_tempOut < 0.0) _showImg(62, 104, symb_temp_minus, sizeof(symb_temp_minus));
    else _showImg(62, 104, symb_temp_plus, sizeof(symb_temp_plus));
}