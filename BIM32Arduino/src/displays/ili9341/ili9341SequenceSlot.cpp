#include <Arduino.h>
#include "./ili9341.hpp"

#include "../config.hpp"

void ILI9341::_sequenceSlotSkip() {
    for(uint8_t i=0; i<4; i++) {
        if(config.display.source.sequence.name(_sequenceSlot) == "") {
            if(_sequenceSlot < 3) _sequenceSlot++;
            else _sequenceSlot = 0;
        }
        else i = 4;
    }
}

void ILI9341::_sequenceSlotNext() {
    if(millis() - _sequenceMillis > config.display.source.sequence.dur() * 1000) {
        _sequenceMillis = millis();
        if(_sequenceSlot < 3) _sequenceSlot++;
        else _sequenceSlot = 0;
    }
}