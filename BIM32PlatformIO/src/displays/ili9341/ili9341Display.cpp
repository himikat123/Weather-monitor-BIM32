#include "./ili9341.hpp"

/**
 * Toggles display (on/off)
 */
void ILI9341::displayToggle() {
    _power = !_power;
}

/**
 * Turns on the display
 */
void ILI9341::displayOn() {
    _power = true;
}

/*
 * Turns off the display
 */
void ILI9341::displayOff() {
    _power = false;
}

/**
 * Returns true if display is on
 */
bool ILI9341::isDisplayOn() const {
    return _power;
}