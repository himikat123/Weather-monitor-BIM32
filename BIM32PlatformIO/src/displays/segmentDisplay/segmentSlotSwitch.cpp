#include <TimeLib.h>

#include "./segmentDisplay.hpp"
#include "../../config/config.hpp"

void SegmentDisplay::_slotSwitch() {
    unsigned int period = config.display.timeSlot.period(_slot, _dispNum);
    if((millis() - _prevSlotMillis) > (period * 1000) or period == 0) {
        _prevSlot = _slot;
        _slot++;
        _animSlot = 0;
        _animMillis = millis();
        for(uint8_t i=_slot; i<8; i++) {
            if(config.display.timeSlot.period(_slot, _dispNum) == 0) {
                _slot++;
                _animSlot = 0;
                _animMillis = millis();
            }
            else break;
        }
        if(_slot > 7) {
            _slot = 0;
            _animSlot = 0;
            _animMillis = millis();
        }
        _prevSlotMillis = millis();
    }
    if(_prevSecond != second()) {
        _millisShift = millis() % 1000;
        _prevSecond = second();
    }
    _pointsState = !((millis() - _millisShift) % (_dotfreq * 2) > _dotfreq);
}