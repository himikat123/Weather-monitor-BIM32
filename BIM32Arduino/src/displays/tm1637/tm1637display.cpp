#include <Arduino.h>
#include "tm1637display.hpp"

#include "../config.hpp"
#include "./tm1637driver.hpp"
#include "./fonts/tm1637Segment.hpp"

/**
 * Display initialize
 * @param dispNum display number
 * @param clk display CLK pin number
 * @param dat display DAT pin number
 */
void TM1637::init(uint8_t dispNum, int8_t clk, int8_t dat) {
    _dispNum = dispNum;
    _clk = clk;
    _dat = dat;
    pinMode(clk, OUTPUT);
    pinMode(dat, OUTPUT);
    digitalWrite(clk, HIGH);
    digitalWrite(dat, HIGH);
    tm1637.init(clk, dat);
    _setModel(config.display.model(dispNum));
    _clearDisplay();
}

/**
 * Displays the next display slot
 */
void TM1637::refresh() {
    _slotSwitch();
    _segAnimations();
    _print();
}

/**
 * Display data on the display
 */
void TM1637::_print() {
    bool updated = false;

    for(uint8_t i=0; i<6; i++) {
        if(_dispImg[i] >= DOT) _pixels[i] = font_tm1637[WITH_DOT][_dispImg[i] - DOT];
        else _pixels[i] = font_tm1637[SYMBOLS][_dispImg[i]];

        if(_pixelsPrev[i] != _pixels[i]) {
            _pixelsPrev[i] = _pixels[i];
            updated |= true;
        }
    }

    if(updated) _sendToDisplay();
}

/**
 * Send data to display
 */
void TM1637::_sendToDisplay() {
    uint8_t bright = map(_brightness, 0, 100, 0, 7);
    bright = constrain(bright, 0, 7);
    byte seg[6] = { 0, 0, 0, 0, 0, 0 };
    if(_power) {
        for(uint8_t i=0; i<6; i++) seg[i] = _pixels[config.display.order(_dispNum, i) - 1];
    }
    tm1637.start();
    tm1637.writeByte(0x40);
    tm1637.stop();
    tm1637.start();
    tm1637.writeByte(0xC0);
    for(uint8_t i=0; i<6; i++) {
        tm1637.writeByte(seg[i]);
    }
    tm1637.stop();
    tm1637.start();
    tm1637.writeByte(0x88 + bright);
    tm1637.stop();
}

/**
 * Clear the display
 */
void TM1637::_clearDisplay() {
    for(int i=0; i<6; i++) _pixels[i] = 0;
    _print();
}