#include "./segmentDisplay.hpp"
#include "../../validation/validate.hpp"

/**
 * Preparing data for displaying the IAQ
 * @param i Index Air Quality
 */
void SegmentDisplay::_iaq(float i, int* segImg) {
    bool valid = validate.iaq(i);
    int iaq = round(i);
    uint8_t i100 = valid ? iaq < 100 ? SYMB_SPACE : floor(iaq / 100) : SYMB_MINUS;
    uint8_t i10 = valid ? iaq < 10 ? SYMB_SPACE : floor(iaq % 100 / 10) : SYMB_MINUS;
    uint8_t i1 = valid ? iaq % 10 : SYMB_MINUS;

    int disp4Img[8] = {
        iaq < 10 ? SYMB_SPACE : SYMB_A, iaq < 100 ? iaq < 10 ? SYMB_A : SYMB_SPACE : i100, 
        iaq < 10 ? SYMB_SPACE : i10, i1, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };

    int disp6Img[8] = {
        SYMB_SPACE, iaq < 10 ? SYMB_SPACE : SYMB_A, 
        iaq < 10 ? SYMB_A : SYMB_SPACE, iaq < 100 ? iaq < 10 ? SYMB_SPACE : i10 : i100,
        iaq < 100 ? i1 : i10, iaq < 100 ? SYMB_SPACE : i1, SYMB_SPACE, SYMB_SPACE
    };

    int disp8Img[8] = {
        SYMB_SPACE, SYMB_SPACE, iaq < 10 ? SYMB_SPACE : SYMB_A, 
        iaq < 10 ? SYMB_A : SYMB_SPACE, iaq < 100 ? iaq < 10 ? SYMB_SPACE : i10 : i100, 
        iaq < 100 ? i1 : i10, iaq < 100 ? SYMB_SPACE : i1, SYMB_SPACE
    };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == 4 ? disp4Img[i] : _dispLength == 6 ? disp6Img[i] : disp8Img[i];
    }
}