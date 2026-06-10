#include "./pixelLedDisplay.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"
#include "../../globals.hpp"

void PixelLed::_sendToDisplay() {
    if(_driver == nullptr) return;

    uint8_t bright = (uint8_t)map(_brightness, 0, 100, 0, 255);
    bright = constrain(bright, 0, 255);
    if(bright < config.display.brightness.min(_dispNum)) bright = config.display.brightness.min(_dispNum);
    if(bright > config.display.brightness.max(_dispNum)) bright = config.display.brightness.max(_dispNum);  
    _currentBrightness = bright; 

    ColorType black(0, 0, 0);
    ColorType rawDotsColor(_reds[0], _greens[0], _blues[0]);
    ColorType dotsColor = ColorType::LinearBlend(black, rawDotsColor, _currentBrightness);
    
    uint8_t lastPixel = 0;
    if(config.display.sLed(_dispNum)) _driver->setPixelColor(lastPixel++, black);
    
    lastPixel = _sendTwoDigits(black, 0, lastPixel);
    _driver->setPixelColor(lastPixel++, (state.disp_on_off[_dispNum] && _points[0] && !_animIsRunnung) ? dotsColor : black);
    _driver->setPixelColor(lastPixel++, (state.disp_on_off[_dispNum] && _points[1] && !_animIsRunnung) ? dotsColor : black);
    lastPixel = _sendTwoDigits(black, 2, lastPixel++); 
    
    if(config.display.model(_dispNum) == 1) {
        _driver->setPixelColor(lastPixel++, (state.disp_on_off[_dispNum] && _points[2] && !_animIsRunnung) ? dotsColor : black);
        _driver->setPixelColor(lastPixel++, (state.disp_on_off[_dispNum] && _points[3] && !_animIsRunnung) ? dotsColor : black);
        lastPixel = _sendTwoDigits(black, 4, lastPixel++);
    }

    _driver->show();
    vTaskDelay(1); 
}
