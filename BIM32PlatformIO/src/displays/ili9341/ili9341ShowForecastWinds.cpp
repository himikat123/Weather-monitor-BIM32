#include "./ili9341.hpp"

#include "../../validation/validate.hpp"
#include "../../languages/languages.hpp"

/**
 * Display daily forecast winds
 */
void ILI9341::_showForecastWinds() {
    for(uint8_t i=0; i<3; i++) {
        if(_prevWinds[i] != _winds[i] || _forced) {
            String wnd = validate.windSpeed(_winds[i]) ? String(int(round(_winds[i]))) + lang.ms() : "--";
            _printText(i * 106 + 31, 224, 44, 15, wnd, FONT1, CENTER, TEXT_COLOR);
            _prevWinds[i] = _winds[i];
        }
    }
}