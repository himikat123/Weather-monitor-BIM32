#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main

#include "./ws2812display.hpp"
#include "../../config/config.hpp"

/**
 * Display initialize
 * @param dispNum display number
 * @param pin display pin number
 */
void WS2812b::init(uint8_t dispNum, uint8_t pin) {
    _dispNum = dispNum;
 
    switch(config.display.model(dispNum)) {
        case 0: _pixelCount = 4 * 7 * (config.display.cntLeds(dispNum) + 1) + 2 + 1; break; // 4 digits, 7 segments, leds per segment, 2 clock points, 1 sacrificial led
        case 1: _pixelCount = 6 * 7 * (config.display.cntLeds(dispNum) + 1) + 2 + 1; break;
        default: _pixelCount = 1; break;
    } 
    

    _strip = dispNum == 0 ? &strip_1 : &strip_2;
    _strip->begin(pin, _pixelCount);
    _strip->clear(true);
    _setModel(config.display.model(dispNum));
}

/**
 * Displays the next display slot
 */
void WS2812b::refresh() {
    _slotSwitch();
    _segAnimations();
    _print();
}