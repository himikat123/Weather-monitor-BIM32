#include <Arduino.h>
#include "./ili9341.hpp"

#include "../../pinout.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

void ILI9341::brightness(unsigned int bright, bool reduc) {
    if(_power) {
        uint8_t brgt = reduc ? round(bright / 2) : bright;
        if(brgt < config.display.brightness.min(0)) brgt = config.display.brightness.min(0);
        if(brgt > config.display.brightness.max(0)) brgt = config.display.brightness.max(0); 
        float r = 100 * log10(2) / log10(255);
        uint16_t br = round(pow(2, (brgt / r)));
        if(br < 1023) analogWrite(TFT_BACKLIGHT, br);
        _prevBright = bright;
    }
    else analogWrite(TFT_BACKLIGHT, 0);
}

void ILI9341::setDotFreq(uint16_t dotFreq) {}