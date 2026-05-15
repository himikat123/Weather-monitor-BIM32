#include <Arduino.h>
#include "./ili9341.hpp"

const byte* ILI9341::_number_picture(uint8_t num) {
    switch (num) {
        case 1: return number_1;
        case 2: return number_2;
        case 3: return number_3;
        case 4: return number_4;
        case 5: return number_5;
        case 6: return number_6;
        case 7: return number_7;
        case 8: return number_8;
        case 9: return number_9;
        default: return number_0;
    }
}

uint16_t ILI9341::_number_picture_size(uint8_t num) {
    switch (num) {
        case 1: return sizeof(number_1);
        case 2: return sizeof(number_2);
        case 3: return sizeof(number_3);
        case 4: return sizeof(number_4);
        case 5: return sizeof(number_5);
        case 6: return sizeof(number_6);
        case 7: return sizeof(number_7);
        case 8: return sizeof(number_8);
        case 9: return sizeof(number_9);
        default: return sizeof(number_0);
    }
}

void ILI9341::_showTime() {
    if(_prevTHour != _tHour || _prevTMinute != _tMinute || _forced) {
        if(_tHour > 9) _showImg(0, 0, _number_picture(_tHour / 10), _number_picture_size(_tHour / 10));
        else {
            if(config.clock.format() % 2 == 0) tft.fillRect(0, 0, 32, 78, BG_COLOR);
            else _showImg(0, 0, _number_picture(0), _number_picture_size(0));
        }
        _showImg(33, 0, _number_picture(_tHour % 10), _number_picture_size(_tHour % 10));
        _prevTHour = _tHour;

        _showImg(77, 0, _number_picture(_tMinute / 10), _number_picture_size(_tMinute / 10));
        _showImg(109, 0, _number_picture(_tMinute % 10), _number_picture_size(_tMinute % 10));
        _prevTMinute = _tMinute;
    }
}

/**
 * Display clock points
 */
void ILI9341::_clockPoints() {
    boolean points = millis() % 1000 > 500;
    tft.fillSmoothCircle(70, 24, 3, points ? HUMIDITY_COLOR : BG_COLOR, BG_COLOR);
    tft.fillSmoothCircle(70, 52, 3, points ? HUMIDITY_COLOR : BG_COLOR, BG_COLOR);
}