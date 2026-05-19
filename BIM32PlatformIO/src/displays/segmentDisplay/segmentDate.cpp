#include <TimeLib.h>

#include "./segmentDisplay.hpp"
#include "../../config/config.hpp"

/**
 * Preparing data for displaying the date
 */
void SegmentDisplay::_date(int* segImg, uint8_t slot) {
    uint8_t dtH = floor(day() / 10), dtL = day() % 10;
    uint8_t mtH = floor(month() / 10), mtL = month() % 10;
    uint8_t yr1 = floor(year() / 1000), yr2 = floor(year() % 1000 / 100), yr3 = floor(year() % 100 / 10), yr4 = floor(year() % 10);

    int disp4Img[8] = {
        dtH, dtL + DOT, mtH, mtL, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };

    int disp6Img[2][8] = {
        {SYMB_SPACE, SYMB_SPACE, dtH, dtL + DOT, mtH, mtL, SYMB_SPACE, SYMB_SPACE},
        {dtH, dtL + DOT, mtH, mtL + DOT, yr3, yr4, SYMB_SPACE, SYMB_SPACE}
    };

    int disp8Img[3][8] = {
        {SYMB_SPACE, SYMB_SPACE, dtH, dtL + DOT, mtH, mtL, SYMB_SPACE, SYMB_SPACE},
        {SYMB_SPACE, dtH, dtL + DOT, mtH, mtL + DOT, yr3, yr4, SYMB_SPACE},
        {dtH, dtL + DOT, mtH, mtL + DOT, yr1, yr2, yr3, yr4}
    };

    uint8_t sens = config.display.timeSlot.data(slot, _dispNum);
    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == 4 ? disp4Img[i] : _dispLength == 6 ? disp6Img[sens][i] : disp8Img[sens][i];
    }
}