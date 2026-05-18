#include "./nextion.hpp"
#include "../../config/config.hpp"

/**
 * Display comfort level
 */
void Nextion::_showComfortLevel() {
    if(_prevComfortType != _comfortType or _forced) {
        _nextion.writeNum("Main.seq.val", _comfortType == 2 ? 1 : 0);
        _prevComfortType = _comfortType;
    }
    if(_prevComfort != _comfort or _forced) {
        if(config.display.source.tempIn.sens() != 4) {
            _nextion.writeStr("Main.comfort.txt", _comfort);
        }
        _prevComfort = _comfort;
    }
}