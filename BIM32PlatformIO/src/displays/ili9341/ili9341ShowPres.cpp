#include "./ili9341.hpp"

#include "../../validation/validate.hpp"
#include "../../languages/languages.hpp"
#include "../../config/config.hpp"

/**
 * Display pressure
 */
void ILI9341::_showPressure() {
    if(_prevPresOut != _presOut || _forced) {
        String buf = validate.pres(_presOut) ? String(int(round(_presOut))) : "--";
        buf += config.units_pres() ? lang.hpa() : lang.mm();
        _printText(250, (config.units_pres() ? 122 : 119), 70, (config.units_pres() ? 16 : 20), buf, FONTPR, CENTER, PRESSURE_COLOR);
        _prevPresOut = _presOut;
    }
}