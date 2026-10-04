#include "./segmentDisplay.hpp"
#include "../../validation/validate.hpp"

/**
 * Preparing data for displaying CO2
 * @param c CO2 level
 */
void SegmentDisplay::_co2(float c, int* segImg) {
    bool valid = validate.co2(c);
    int co2 = round(c);

    for(uint8_t i=0; i<8; i++) segImg[i] = SYMB_SPACE;

    if(!valid || co2 < 0 || (_dispLength == 4 && co2 > 9999)) {
        segImg[0] = SYMB_C; 
        segImg[1] = 0; 
        segImg[2] = 2; 
        segImg[3] = SYMB_MINUS;
        if(_dispLength >= 6) { 
            segImg[3] = SYMB_SPACE; 
            segImg[4] = SYMB_MINUS; 
            segImg[5] = SYMB_MINUS; 
        }
        if(_dispLength == 8) { 
            segImg[6] = SYMB_MINUS; 
            segImg[7] = SYMB_MINUS; 
        }
        return;
    }

    uint8_t digits[5] = {
        (uint8_t)(co2 / 10000),
        (uint8_t)((co2 % 10000) / 1000),
        (uint8_t)((co2 % 1000) / 100),
        (uint8_t)((co2 % 100) / 10),
        (uint8_t)(co2 % 10)
    };

    uint8_t len = co2 < 10 ? 1 : co2 < 100 ? 2 : co2 < 1000 ? 3 : co2 < 10000 ? 4 : 5;

    if(_dispLength == 4) {
        if(len == 1) { 
            segImg[0] = SYMB_C; 
            segImg[1] = 0; 
            segImg[2] = SYMB_SPACE; 
            segImg[3] = digits[4]; 
        }
        if(len == 2) { 
            segImg[0] = SYMB_C; 
            segImg[1] = SYMB_SPACE; 
            segImg[2] = digits[3]; 
            segImg[3] = digits[4]; 
        }
        if(len == 3) { 
            segImg[0] = SYMB_C; 
            segImg[1] = digits[2]; 
            segImg[2] = digits[3]; 
            segImg[3] = digits[4]; 
        }
        if(len == 4) { 
            segImg[0] = digits[1]; 
            segImg[1] = digits[2]; 
            segImg[2] = digits[3]; 
            segImg[3] = digits[4]; 
        }
    }

    else if(_dispLength == 6) {
        uint8_t numStart = 6 - len; 
        for(uint8_t i=0; i<len; i++) segImg[numStart + i] = digits[5 - len + i];
        if(len == 1 || len == 2) { 
            segImg[0] = SYMB_C; 
            segImg[1] = 0; 
            segImg[2] = 2; 
        }
        if(len == 3) { 
            segImg[0] = SYMB_C; 
            segImg[1] = 0; 
        }
        if(len == 4) { 
            segImg[0] = SYMB_C; 
        }
    }

    else if(_dispLength == 8) {
        uint8_t numStart = 8 - len;
        for(uint8_t i=0; i<len; i++) segImg[numStart + i] = digits[5 - len + i];
        if(len >= 1 && len <= 4) { 
            segImg[0] = SYMB_C; 
            segImg[1] = 0; 
            segImg[2] = 2; 
        }
        if(len == 5) { 
            segImg[0] = SYMB_C; 
            segImg[1] = 0; 
        }
    }
}