#include <TimeLib.h>

#include "./segmentDisplay.hpp"
#include "../../config/config.hpp"

int SegmentDisplay::_pendulumPattern(uint32_t ml, uint8_t max) {
    float phase = (ml % 2000) / (2000 / (2 * max));
    return phase <= max ? round(phase) : round(2 * max - phase);
}

/**
 * Preparing data for displaying the clock
 */
void SegmentDisplay::_clock(int* segImg, uint8_t slot) {
    const uint8_t POINTS_BLINK_TOGETHER = 0;
    const uint8_t POINTS_BLINK_IN_TURN = 1;
    const uint8_t POINTS_ALLWAYS_ON = 2;
    const uint8_t POINTS_ALLWAYS_OFF = 3;

    uint8_t hr = config.clock.format() > 1 ? hour() : hourFormat12();
    uint8_t hrH = floor(config.clock.format() % 2 == 0 && hr < 10 ? SYMB_SPACE : hr / 10);
    uint8_t hrL = hr % 10;
    uint8_t mnH = floor(minute() / 10), mnL = minute() % 10;
    uint8_t scH = floor(second() / 10), scL = second() % 10;
    int64_t ml = millis() - _millisShift;
    uint8_t msH = floor(ml % 1000 / 100), msL = floor(ml % 100 / 10);
    int pendulum = _pendulumPattern(second() * 1000 + (ml % 1000), _dispLength - 1);
    bool point1 = false, point2 = false;

    switch(config.display.animation.points(_dispNum)) {
        case POINTS_BLINK_TOGETHER: point1 = point2 = _pointsState; break;
        case POINTS_BLINK_IN_TURN: {
            point1 = config.display.type(_dispNum) == 2 ? _pointsState : false; 
            point2 = config.display.type(_dispNum) == 2 ? !point1 : false; 
        } break;
        case POINTS_ALLWAYS_ON: point1 = point2 = true; break;
        case POINTS_ALLWAYS_OFF: point1 = point2 = false; break;
        default: ; break;
    }

    int disp4Img[8] = {
        config.display.type(_dispNum) == 2 && point1 ? (hrH + DOT) : hrH, 
        point2 ? (hrL + DOT) : hrL, 
        mnH, mnL, 
        SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
    };

    int disp6Img[3][8] = {
        {
            SYMB_SPACE, SYMB_SPACE, config.display.type(_dispNum) == 2 && point1 ? (hrH + DOT) : hrH, 
            point2 ? (hrL + DOT) : hrL, mnH, mnL, SYMB_SPACE, SYMB_SPACE
        },
        {
            config.display.type(_dispNum) == 2 && point1 ? (hrH + DOT) : hrH, point2 ? (hrL + DOT) : hrL, 
            config.display.type(_dispNum) == 2 && point1 ? (mnH + DOT) : mnH, point2 ? (mnL + DOT) : mnL, 
            scH, scL, SYMB_SPACE, SYMB_SPACE
        },
        {SYMB_SPACE, hrH, hrL, point1 ? SYMB_MINUS : SYMB_SPACE, mnH, mnL}
    };

    int disp8Img[5][8] = {
        {SYMB_SPACE, SYMB_SPACE, hrH, point1 ? hrL + DOT : hrL, mnH, mnL, SYMB_SPACE, SYMB_SPACE},
        {SYMB_SPACE, hrH, point1 ? hrL + DOT : hrL, mnH, point1 ? mnL + DOT : mnL, scH, scL, SYMB_SPACE},
        {SYMB_SPACE, SYMB_SPACE, hrH, hrL, point1 ? SYMB_MINUS : SYMB_SPACE, mnH, mnL, SYMB_SPACE},
        {hrH, hrL, point1 ? SYMB_MINUS : SYMB_SPACE, mnH, mnL, point1 ? SYMB_MINUS : SYMB_SPACE, scH, scL},
        {hrH, point1 ? hrL + DOT : hrL, mnH, point1 ? mnL + DOT : mnL, scH, point1 ? scL + DOT : scL, msH, msL}
    };

    uint8_t sens = config.display.timeSlot.data(slot, _dispNum);
    for(uint8_t i=0; i<8; i++) {
        segImg[i] = _dispLength == 4 ? disp4Img[i] : _dispLength == 6 ? disp6Img[sens][i] : disp8Img[sens][i];
    }

    if(config.display.animation.points(_dispNum) == 1 && config.display.type(_dispNum) != 2) {
        for(uint8_t i=0; i<_dispLength; i++) {
            segImg[i] = pendulum == i ? segImg[i] + DOT : segImg[i];
        }
    }
}