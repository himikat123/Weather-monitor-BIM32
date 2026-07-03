#include <Arduino.h>
#include "./pixelLedDisplay.hpp"

#include "../../state/state.hpp"
#include "../../config/config.hpp"
#include "../../globals.hpp"

/**
 * Change display brightness
 */
void PixelLed::brightness(unsigned int bright, bool reduc) {
    _brightness = reduc ? round(bright / 2) : bright;
    if(_brightness != _prevBrightness) {
        _prevBrightness = _brightness;
        _sendToDisplay();
    }
}