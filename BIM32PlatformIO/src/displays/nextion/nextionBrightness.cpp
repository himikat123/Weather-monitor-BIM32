#include <Arduino.h>
#include "./nextion.hpp"

#include "../../state/state.hpp"
#include "../../config/config.hpp"
#include "../../globals.hpp"

/**
 * Change display brightness
 */
void Nextion::brightness(unsigned int bright, bool reduc) {
    uint8_t br = reduc ? round(bright / 2) : bright;
    if(br < config.display.brightness.min(DISPLAY_1)) br = config.display.brightness.min(DISPLAY_1);
    if(br > config.display.brightness.max(DISPLAY_1)) br = config.display.brightness.max(DISPLAY_1); 
    if(_prevBright != br or _forced) {
        if(state.disp_on_off[DISPLAY_1]) _nextion.writeNum("dim", br);
        _prevBright = br;
        _forced = false;
    }
}

void Nextion::setDotFreq(uint16_t dotFreq) {}