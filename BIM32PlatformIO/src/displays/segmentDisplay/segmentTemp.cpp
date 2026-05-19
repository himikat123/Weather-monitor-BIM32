#include "./segmentDisplay.hpp"
#include "../../validation/validate.hpp"

/**
 * Preparing data for displaying the temperature
 * @param t temperature
 */
void SegmentDisplay::_temp(float t, int* segImg) {
    bool valid = validate.temp(t);
    int tmp = round(t);
    uint8_t th = floor(abs(tmp) / 10), tl = abs(tmp) % 10;
    if(th == 0) th = SYMB_SPACE;
    uint8_t c = SYMB_C;

    int disp4Img[8] = {
        valid ? (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? th : tl : tl) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE) : SYMB_DEGREE,
        valid ? (tmp < 0 ? tmp < -9 ? SYMB_DEGREE : c : c) : c,
        SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };

    int disp6Img[8] = {
        SYMB_SPACE,
        valid ? (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? th : tl : tl) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE) : SYMB_DEGREE,
        valid ? (tmp < -9 ? SYMB_DEGREE : c) : c,
        valid ? (tmp < -9 ? c : SYMB_SPACE) : SYMB_SPACE,
        SYMB_SPACE, SYMB_SPACE
    };

    int disp8Img[8] = {
        SYMB_SPACE, SYMB_SPACE,
        valid ? (tmp < 0 ? SYMB_MINUS : tmp > 9 ? th : SYMB_SPACE) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? th : tl : tl) : SYMB_MINUS,
        valid ? (tmp < 0 ? tmp < -9 ? tl : SYMB_DEGREE : SYMB_DEGREE) : SYMB_DEGREE,
        valid ? (tmp < -9 ? SYMB_DEGREE : c) : c,
        valid ? (tmp < -9 ? c : SYMB_SPACE) : SYMB_SPACE,
        SYMB_SPACE
    };

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == 4 ? disp4Img[i] : _dispLength == 6 ? disp6Img[i] : disp8Img[i];
    }
}