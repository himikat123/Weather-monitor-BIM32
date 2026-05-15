#include "./nextion.hpp"

/**
 * Display current weather icon
 */
void Nextion::_showCurrentIcon() {
    if(_prevCurrIcon != _currIcon or _prevIsDay != _isDay or _forced) {
        uint8_t icon = 64;
        switch(_currIcon) {
            case 1: icon = _isDay ? 10 : 11; break;
            case 2: icon = _isDay ? 12 : 13; break;
            case 3: icon = 14; break;
            case 4: icon = 15; break;
            case 5: icon = 16; break;
            case 6: icon = _isDay ? 17 : 18; break;
            case 7: icon = 19; break;
            case 8: icon = 20; break;
            default: ; break;
        }
        _nextion.writeNum("Main.icon0.pic", icon);
        _prevCurrIcon = _currIcon;
        _prevIsDay = _isDay;
    }
}