#include "./segmentDisplay.hpp"
#include "../../config/config.hpp"
#include "../../validation/validate.hpp"

/**
 * Preparing data for displaying the pressure
 * @param p pressure
 */
void SegmentDisplay::_pres(float pres, int* segImg) {
    int p = round(pres);
    bool valid = validate.pres(pres);
    uint8_t p1000 = valid ? floor(p / 1000) : SYMB_MINUS;
    uint8_t p100 = valid ? floor(p % 1000 / 100) : SYMB_MINUS;
    uint8_t p10 = valid ? floor(p % 100 / 10) : SYMB_MINUS;
    uint8_t p1 = valid ? p % 10 : SYMB_MINUS;
    bool m = config.units_pres() && p > 999;

    int disp4Img[8] = { m ? p1000 : p100, m ? p100 : p10, m ? p10 : p1, m ? p1 : SYMB_P, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE };
    int disp6Img[8] = { p1000 == 0 ? SYMB_SPACE : p1000, p100, p10, p1, SYMB_SPACE, SYMB_P, SYMB_SPACE, SYMB_SPACE };
    int disp8Img[8] = { SYMB_SPACE, p1000 == 0 ? SYMB_SPACE : p1000, p100, p10, p1, SYMB_SPACE, SYMB_P, SYMB_SPACE };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == 4 ? disp4Img[i] : _dispLength == 6 ? disp6Img[i] : disp8Img[i];
    }
}