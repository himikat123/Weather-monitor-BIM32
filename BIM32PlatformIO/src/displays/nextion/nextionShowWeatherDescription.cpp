#include "./nextion.hpp"

/**
 * Display weather description
 */
void Nextion::_showWeatherDescription() {
    if(_prevDescription != _description or _forced) {
        _nextion.writeStr("Main.description.txt", _description == "" ? "--" : _description);
        _prevDescription = _description;
    }
}