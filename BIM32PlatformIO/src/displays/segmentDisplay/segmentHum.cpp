#include "./segmentDisplay.hpp"
#include "../../validation/validate.hpp"

/**
 * Preparing data for displaying the humidity
 * @param h humidity
 */
void SegmentDisplay::_hum(float h, int* segImg) {
    bool valid = validate.hum(h);
    int hm = round(h);
    uint8_t hh = floor(hm / 10), hl = hm % 10;
    if(hh == 0) hh = SYMB_SPACE;

    int disp4Img[8] = {
        valid ? (hm > 99 ? 1 : hh) : SYMB_MINUS, valid ? (hm > 99 ? 0 : hl) : SYMB_MINUS,
        valid ? (hm > 99 ? 0 : SYMB_SPACE) : SYMB_SPACE, SYMB_H, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };

    int disp6Img[8] = {
        SYMB_SPACE, valid ? (hm > 9 ? hm > 99 ? 1 : hh : SYMB_SPACE) : SYMB_MINUS,
        valid ? (hm > 99 ? 0 : hl) : SYMB_MINUS, valid ? (hm > 99 ? 0 : SYMB_SPACE) : SYMB_SPACE,
        valid ? (hm > 99 ? SYMB_SPACE : SYMB_H) : SYMB_H, valid ? (hm > 99 ? SYMB_H : SYMB_SPACE) : SYMB_SPACE, 
        SYMB_SPACE, SYMB_SPACE
    };

    int disp8Img[8] = {
        SYMB_SPACE, SYMB_SPACE, valid ? (hm > 9 ? hm > 99 ? 1 : hh : SYMB_SPACE) : SYMB_MINUS,
        valid ? (hm > 99 ? 0 : hl) : SYMB_MINUS, valid ? (hm > 99 ? 0 : SYMB_SPACE) : SYMB_SPACE,
        valid ? (hm > 99 ? SYMB_SPACE : SYMB_H) : SYMB_H, valid ? (hm > 99 ? SYMB_H : SYMB_SPACE) : SYMB_SPACE, SYMB_SPACE
    };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == 4 ? disp4Img[i] : _dispLength == 6 ? disp6Img[i] : disp8Img[i];
    }
}