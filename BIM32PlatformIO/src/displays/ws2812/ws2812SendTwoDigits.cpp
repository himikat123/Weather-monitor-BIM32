#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main

#include "./ws2812display.hpp"
#include "../../config/config.hpp"

uint8_t WS2812b::_sendTwoDigits(rgb_t black, uint8_t digShift, uint8_t pixelNr) {
    uint8_t repeats = config.display.cntLeds(_dispNum) + 1;

    for(uint8_t digNr=0; digNr<2; digNr++) {
        for(uint8_t bitNr=0; bitNr<7; bitNr++) {
            for(uint8_t repeat=0; repeat<repeats; repeat++) {
                uint8_t imgNr = digNr + digShift;

                if(bitRead(_pixels[imgNr], bitNr) and _power) {
                    rgb_t color = { .r = _reds[imgNr], .g = _greens[imgNr], .b = _blues[imgNr] };
                    _strip->setPixel(pixelNr++, color, false);
                }
                else _strip->setPixel(pixelNr++, black, false);
            }
        }
    }

    return pixelNr;
}