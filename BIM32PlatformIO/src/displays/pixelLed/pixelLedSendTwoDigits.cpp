#include "./pixelLedDisplay.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"
#include "../../globals.hpp"

uint8_t PixelLed::_sendTwoDigits(ColorType black, uint8_t digShift, uint8_t pixelNr) {
    uint8_t repeats = config.display.cntLeds(_dispNum) + 1;

    for(uint8_t digNr=0; digNr<2; digNr++) {
        for(uint8_t bitNr=0; bitNr<7; bitNr++) {
            for(uint8_t repeat=0; repeat<repeats; repeat++) {
                uint8_t imgNr = digNr + digShift;

                if(bitRead(_pixels[imgNr], bitNr) && state.disp_on_off[_dispNum]) {
                    ColorType rawColor(_reds[imgNr], _greens[imgNr], _blues[imgNr]);
                    ColorType color = ColorType::LinearBlend(black, rawColor, _currentBrightness);
                    _driver->setPixelColor(pixelNr++, color);
                }
                else _driver->setPixelColor(pixelNr++, black);
            }
        }
    }

    return pixelNr;
}
