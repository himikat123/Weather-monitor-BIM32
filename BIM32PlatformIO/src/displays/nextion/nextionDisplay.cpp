#include "./nextion.hpp"
#include "../../state/state.hpp"
#include "../../globals.hpp"

/**
 * Toggle display (on/off)
 */
void Nextion::displayToggle() {
    if(state.disp_on_off[DISPLAY_1]) _nextion.writeNum("dim", 0);
    else init();
    state.disp_on_off[DISPLAY_1] = !state.disp_on_off[DISPLAY_1];
}

/**
 * Turn on the display
 */
void Nextion::displayOn(bool doinit) {
    state.disp_on_off[DISPLAY_1] = true;
    if(doinit) init();
}

/*
 * Turn off the display
 */
void Nextion::displayOff() {
    _nextion.writeNum("dim", 0);
    state.disp_on_off[DISPLAY_1] = false;
}

/**
 * Return true if display is on
 */
bool Nextion::isdisplayOn() const {
    return state.disp_on_off[DISPLAY_1];
}