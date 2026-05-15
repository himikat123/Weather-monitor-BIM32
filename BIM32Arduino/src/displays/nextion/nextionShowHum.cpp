#include "./nextion.hpp"

#include "../validate.hpp"
#include "../config.hpp"

/**
 * Display humidity inside
 */
void Nextion::_showHumIn() {
    if(_prevHumIn != _humIn or _forced) {
        if(config.display.source.humIn.sens() != 4) {
            _nextion.writeStr("Main.humInside.txt", validate.hum(_humIn) 
                ? (String(int(round(_humIn))) + "%") : "--"
            );
        }
        _prevHumIn = _humIn;
    }
}

/**
 * Display humidity outside
 */
void Nextion::_showHumOut() {
    if(_prevHumOut != _humOut or _forced) {
        _nextion.writeStr("Main.humOutside.txt", validate.hum(_humOut) 
            ? (String(int(round(_humOut))) + "%") : "--"
        );
        _prevHumOut = _humOut;
    }
}