#include <TimeLib.h>

#include "./segmentDisplay.hpp"

/**
 * Preparing data for displaying AP mode
 */
void SegmentDisplay::_apMode(int* segImg) {
    int disp4Img[8] = {SYMB_SPACE, SYMB_A, SYMB_P, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int disp6Img[8] = {SYMB_SPACE, SYMB_SPACE, SYMB_A, SYMB_P, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int disp8Img[8] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_A, SYMB_P, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};

    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == 4 ? disp4Img[i] : _dispLength == 6 ? disp6Img[i] : disp8Img[i];
    }
}