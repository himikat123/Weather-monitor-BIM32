#include "./ws2812display.hpp"
#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main
#include "../../config.hpp"

/**
 * Send data to display
 */
void WS2812b::_sendToDisplay() {
    uint8_t bright = (uint8_t)map(_brightness, 0, 100, 0, 255);
    bright = constrain(bright, 0, 255);
    if(bright < config.display.brightness.min(_dispNum)) bright = config.display.brightness.min(_dispNum);
    if(bright > config.display.brightness.max(_dispNum)) bright = config.display.brightness.max(_dispNum);  
    _strip->brightness(bright, false);

    rgb_t black = { .r = 0, .g = 0, .b = 0 };
    rgb_t dotsColor = { .r = _reds[0], .g = _greens[0], .b = _blues[0] };
    _strip->setPixel(0, black, false);
    uint8_t lastPixel = _sendTwoDigits(black, 0, 1);
    _strip->setPixel(lastPixel++, (_power && _points[0] && !_animIsRunnung) ? dotsColor : black, false);
    _strip->setPixel(lastPixel++, (_power && _points[1] && !_animIsRunnung) ? dotsColor : black, false);
    lastPixel = _sendTwoDigits(black, 2, lastPixel++);

    if(config.display.model(_dispNum) > 2) {
        _strip->setPixel(lastPixel++, (_power && _points[2] && !_animIsRunnung) ? dotsColor : black, false);
        _strip->setPixel(lastPixel++, (_power && _points[3] && !_animIsRunnung) ? dotsColor : black, false);
        lastPixel = _sendTwoDigits(black, 4, lastPixel++);
    }

    _strip->show();
    vTaskDelay(1);
}