#include "./segmentDisplay.hpp"
#include "../../validation/validate.hpp"

/**
 * Preparing data for displaying CO2
 * @param c CO2 level
 */
void SegmentDisplay::_co2(float c, int* segImg) {
    bool valid = validate.co2(c);
    int co2 = round(c);
    uint8_t c1000 = co2 < 1000 ? SYMB_SPACE : floor(co2 / 1000);
    uint8_t c100 = co2 < 100 ? SYMB_SPACE : floor(co2 % 1000 / 100);
    uint8_t c10 = co2 < 10 ? SYMB_SPACE : floor(co2 % 100 / 10);
    uint8_t c1 = co2 % 10;

    int disp4Img[8] = {
        valid ? co2 < 1000 ? SYMB_C : c1000 : SYMB_C, valid ? co2 < 100 ? 0 : c100 : 0, 
        valid ? co2 < 10 ? SYMB_SPACE : c10 : 2, valid ? c1 : SYMB_MINUS, 
        SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };

    int disp6Img[8] = {
        co2 < 10 ? SYMB_SPACE : SYMB_C, co2 < 10 ? SYMB_C : 0, 
        valid ? co2 < 1000 ? co2 < 100 ? co2 < 10 ? 0 : 2 : SYMB_SPACE : c1000 : 2, 
        valid ? co2 < 100 ? co2 < 10 ? 2 : SYMB_SPACE : c100 : SYMB_SPACE, 
        valid ? co2 < 10 ? SYMB_SPACE : c10 : SYMB_MINUS, valid ? c1 : SYMB_MINUS, 
        SYMB_SPACE, SYMB_SPACE
    };

    int disp8Img[8] = {
        valid ? co2 < 1000 ? SYMB_SPACE : SYMB_C : SYMB_C,
        valid ? co2 < 1000 ? co2 < 10 ? SYMB_SPACE : SYMB_C : 0 : 0,
        valid ? co2 < 1000 ? co2 < 10 ? SYMB_C : 0 : 2 : 2,
        valid ? co2 < 1000 ? co2 < 10 ? 0 : 2 : SYMB_SPACE : SYMB_SPACE,
        valid ? co2 < 1000 ? co2 < 10 ? 2 : SYMB_SPACE : c1000 : SYMB_MINUS,
        valid ? co2 < 100 ? co2 < 10 ? SYMB_SPACE : c10 : c100 : SYMB_MINUS,
        valid ? co2 < 100 ? c1 : c10 : SYMB_MINUS,
        valid ? co2 < 100 ? SYMB_SPACE : c1 : SYMB_MINUS
    };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == 4 ? disp4Img[i] : _dispLength == 6 ? disp6Img[i] : disp8Img[i];
    }
}