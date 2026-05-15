#include "./ili9341.hpp"

#include "../config.hpp"
#include "../validate.hpp"

/**
 * Display humidity
 */
void ILI9341::_showHumidity(int hum, uint16_t x, uint16_t y) {
    String buf = validate.hum(hum) ? (String(hum)) : "--";
    buf += "%";
    _printText(x, y, 58, 20, buf, FONT2, CENTER, HUMIDITY_COLOR);
}

/**
 * Display humidity inside
 */
void ILI9341::_showHumidityInside() {
    if(config.display.source.humIn.sens() == 4) _humIn = _humSequence[_sequenceSlot];
    if(_prevHumIn != _humIn || _forced) {
        _showHumidity(int(round(_humIn)), 264, 58);
        _prevHumIn = _humIn;
    }
}

/**
 * Display humidity outside
 */
void ILI9341::_showHumidityOutside() {
    if(_prevHumOut != _humOut || _forced) {
        _showHumidity(int(round(_humOut)), 164, 119);
        _prevHumOut = _humOut;
    }
}