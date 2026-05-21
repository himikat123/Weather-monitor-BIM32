#include "./nextion.hpp"

/**
 * Toggle display (on/off)
 */
void Nextion::displayToggle() {
    if(_power) _nextion.writeNum("dim", 0);
    else init();
    _power = !_power;
}

/**
 * Turn on the display
 */
void Nextion::displayOn(bool doinit) {
    _power = true;
    if(doinit) init();
}

/*
 * Turn off the display
 */
void Nextion::displayOff() {
    _nextion.writeNum("dim", 0);
    _power = false;
}

/**
 * Return true if display is on
 */
bool Nextion::isdisplayOn() const {
    return _power;
}