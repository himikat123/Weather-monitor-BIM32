#include <Arduino.h>
#include "./ili9341.hpp"

#include "../../config/config.hpp"

/**
 * Display comfort level
 */
void ILI9341::_showComfort() {
    if(config.display.source.descr() == 2) _comfort = _nameSequence[_sequenceSlot];
    else {
        if(_comfort.indexOf(".") > 0) {
            char buf[255];
            _comfort.toCharArray(buf, 255);
            char* cmf0 = strtok(buf, ".");
            char* cmf1 = strtok(NULL, ".");
            if(millis() - _prevCmfTime >= 2000) {
                _prevCmfTime = millis();
                _cmfType = !_cmfType;
                for(size_t i = 0; cmf1[i] != '\0'; i++) cmf1[i] = cmf1[i + 1]; // remove first space character
                _printText(145, 28, 174, 16, String(_cmfType ? cmf1 : cmf0), FONT1, CENTER, TEXT_COLOR);
            }
        }
        if(_prevComfort != _comfort || _forced) {
            _printText(145, 28, 174, 16, _comfort, FONT1, CENTER, TEXT_COLOR);
            _prevComfort = _comfort;
        }
    }
}