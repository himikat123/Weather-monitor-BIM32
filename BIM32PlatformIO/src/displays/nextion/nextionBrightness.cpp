#include "./nextion.hpp"

#include "../../state/state.hpp"
#include "../../config/config.hpp"

/**
 * Change display brightness
 */
void Nextion::brightness(unsigned int bright) {
    uint8_t br = state.reduc[0] ? round(bright / 2) : bright;
    if(br < config.display.brightness.min(DISPLAY_1)) br = config.display.brightness.min(DISPLAY_1);
    if(br > config.display.brightness.max(DISPLAY_1)) br = config.display.brightness.max(DISPLAY_1); 
    if(_prevBright != br or _forced) {
        if(_power) _nextion.writeNum("dim", br);
        _prevBright = br;
        _forced = false;
    }
}