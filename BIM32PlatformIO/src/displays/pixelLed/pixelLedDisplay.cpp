#include "./pixelLedDisplay.hpp"
#include "./ws2812Drive.hpp"
#include "./sk9822Drive.hpp"
#include "../../config/config.hpp"
#include "../../globals.hpp"

void PixelLed::init(uint8_t dispNum, uint8_t pin) {
    _dispNum = dispNum;
 
    uint8_t cntLeds = config.display.cntLeds(dispNum) + 1;
    switch(config.display.model(dispNum)) {
        case 0: _pixelCount = 4 * 7 * cntLeds + 2; break; 
        case 1: _pixelCount = 6 * 7 * cntLeds + 4; break;
        case 2: _pixelCount = 4 * 7 * cntLeds + 2; break; 
        case 3: _pixelCount = 6 * 7 * cntLeds + 4; break;
        case 4: _pixelCount = 8 * 7 * cntLeds + 6; break; 
        default: _pixelCount = 1; break;
    }
    if(config.display.sLed(dispNum)) _pixelCount++;

    if(_driver) delete _driver;
    
    if(config.display.model(dispNum) <= D_WS2812_6) _driver = new Ws2812Drive();
    if(config.display.model(dispNum) >= D_SK9822_4) _driver = new Sk9822Drive();

    _driver->begin(dispNum, _pixelCount);
    _driver->clearTo(ColorType(0, 0, 0));
    _driver->show();

    _setModel(config.display.model(dispNum));
}

void PixelLed::refresh() {
    _slotSwitch();
    _segAnimations();
    _print(); 
}