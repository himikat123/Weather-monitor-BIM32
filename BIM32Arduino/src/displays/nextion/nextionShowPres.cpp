#include "./nextion.hpp"

#include "../../validation/validate.hpp"
#include "../../config/config.hpp"
#include "../../languages/languages.hpp"
#include "../../sensors/sensors.hpp"

/**
 * Display pressure
 */
void Nextion::_showPres() {
    if(_prevPresOut != _presOut or _forced) {
        int presInt = round(config.units_pres() ? _presOut : sensors.mmHg(_presOut));
        String buf = validate.pres(_presOut) ? String(presInt) : "--";
        buf += config.units_pres() ? lang.hpa() : lang.mm();
        _nextion.writeStr("Main.presOutside.txt", buf);
        _nextion.writeNum("Main.presOutside.font", presInt < 1000 ? 2 : 1);
        _prevPresOut = _presOut;
    }
}