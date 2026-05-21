#include <Arduino.h>
#include "./max7219segmentDisplay.hpp"

#include "../../config/config.hpp"
#include "./fonts/max7219Segment.hpp"

/**
 * Display initialize
 * @param dispNum display number
 * @param clk display CLK pin number
 * @param dat display DAT pin number
 * @param load display LOAD pin number
 */
void MAX7219_S::init(uint8_t dispNum, int8_t clk, int8_t dat, int8_t load) {
    _dispNum = dispNum;
    _clk = clk;
    _dat = dat;
    _load = load;
    pinMode(dat, OUTPUT);
    pinMode(clk, OUTPUT);
    pinMode(load, OUTPUT);
    digitalWrite(load, HIGH);
    max7219.init(clk, dat, load, 0);
    _setModel(config.display.model(dispNum));
}

/**
 * Displays the next display slot
 */
void MAX7219_S::refresh() {
    _slotSwitch();
    _segAnimations();
    _print();
}

/**
 * Display data on the display
 */
void MAX7219_S::_print() {
    bool updated = false;

    for(uint8_t i=0; i<8; i++) {
        if(_dispImg[i] >= 100) _pixels[i] = font_max7219[1][_dispImg[i] - 100];
        else _pixels[i] = font_max7219[0][_dispImg[i]];

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
void MAX7219_S::_sendToDisplay() {
    byte seg[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    if(_power) {
        for(uint8_t i=0; i<8; i++) seg[i] = _pixels[config.display.order(_dispNum, i) - 1];
    }
    max7219.print(seg);
}

/**
 * Clear the display
 */
void MAX7219_S::_clearDisplay() {
    for(int i=0; i<8; i++) _pixels[i] = 0;
    _print();
}

/**
 * Toggle display on/off
 */
void MAX7219_S::displayToggle() {
    _power = !_power;
    max7219.power(_power);
}

/**
 * Turn on the display
 */
void MAX7219_S::displayOn(bool doinit) {
    _power = true;
    max7219.power(true);
}

/*
 * Turn off the display
 */
void MAX7219_S::displayOff() {
    _power = false;
    max7219.power(false);
}

bool MAX7219_S::isdisplayOn() const {
    return _power;
}


/**
 * Set display brightness
 * @param brightness intensity
 * @param reduc brightness is reduced or not
 */
void MAX7219_S::brightness(uint8_t intensity, bool reduc) {
    _brightness = reduc ? round(intensity / 2) : intensity;
    uint8_t bright = map(_brightness, 0, 100, 0, 15);
    bright = constrain(bright, 0, 15);
    max7219.setIntensity(bright);
}