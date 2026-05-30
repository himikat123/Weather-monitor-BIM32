#include "./pixelLedDisplay.hpp"
#include "./fonts/ws2812Segment.hpp"
#include "../../globals.hpp"

/**
 * Display data on the display
 */
void PixelLed::_print() {
    bool updated = false;

    for(uint8_t i=0; i<6; i++) {
        _points[i] = _dispImg[i] >= DOT;
        _pixels[i] = font_ws2812[_dispImg[i] >= DOT ? _dispImg[i] - DOT : _dispImg[i]];
        unsigned int colors = strtol(&_dispColors[i][1], NULL, 16);
        _reds[i] = colors >> 16;
        _greens[i] = colors >> 8 & 0xFF;
        _blues[i] = colors & 0xFF;

        if(_pixelsPrev[i] != _pixels[i] or _redsPrev[i] != _reds[i] or
           _greensPrev[i] != _greens[i] or _bluesPrev[i] != _blues[i]
        ) {
            _pixelsPrev[i] = _pixels[i];
            _redsPrev[i] = _reds[i];
            _greensPrev[i] = _greens[i];
            _bluesPrev[i] = _blues[i];
            updated |= true;
        }
    }

    for(uint8_t i=0; i<4; i++) {
        if(_prevPoints[i] != _points[i]) {
            _prevPoints[i] = _points[i];
            updated |= true;
        }
    }

    if(updated) _sendToDisplay();
}