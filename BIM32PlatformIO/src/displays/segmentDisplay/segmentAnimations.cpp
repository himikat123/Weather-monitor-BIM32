#include "./segmentDisplay.hpp"
#include "./segmentAnimationsShifts.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

void SegmentDisplay::_segAnimations() {
    int segImg[8] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE};
    int segImgPrev[8] = {SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE}; 

    _segGetData(segImgPrev, _prevSlot, false);
    _segGetData(segImg, _slot, true);
    String color = config.display.timeSlot.color(_slot, _dispNum);
    String prevColor = config.display.timeSlot.color(_prevSlot, _dispNum);
    if(state.apMode) {
        color = "#FFFFFF";
        prevColor = "#FFFFFF";
    }

    _animIsRunnung = true;
    unsigned int type = config.display.animation.type(_dispNum);
    uint8_t dl = _dispLength == 4 ? 0 : _dispLength == 6 ? 1 : 2;

    for(uint8_t i=0; i<(_dispLength); i++) {
        uint8_t shf = abs(SHIFTS[dl][type][_animSlot][i]) - 1;
        if(SHIFTS[dl][type][_animSlot][i] == 0) _dispImg[i] = SYMB_SPACE;
        else {
            if(SHIFTS[dl][type][_animSlot][i] < 0) _dispImg[i] = segImgPrev[shf]; 
            else _dispImg[i] = segImg[shf];
        }
        
        if(SHIFTS[dl][type][_animSlot][i] < 0) prevColor.toCharArray(_dispColors[i], 8);
        else color.toCharArray(_dispColors[i], 8);
    }

    if(millis() - _animMillis > 1000 / config.display.animation.speed(_dispNum)) {
        _animMillis = millis();
        if(_animSlot < FRAMES[dl][type] - 1) _animSlot++;
    }

    if(_animSlot >= FRAMES[dl][type] - 1) _animIsRunnung = false;
}