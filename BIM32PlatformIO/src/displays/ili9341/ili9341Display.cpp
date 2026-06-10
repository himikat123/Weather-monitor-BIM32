#include "./ili9341.hpp"
#include "../../state/state.hpp"
#include "../../globals.hpp"

/**
 * Toggles display (on/off)
 */
void ILI9341::displayToggle() {
    state.disp_on_off[DISPLAY_1] = !state.disp_on_off[DISPLAY_1];
}

/**
 * Turns on the display
 */
void ILI9341::displayOn(bool doinit) {
    state.disp_on_off[DISPLAY_1] = true;
}

/*
 * Turns off the display
 */
void ILI9341::displayOff() {
    state.disp_on_off[DISPLAY_1] = false;
}

/**
 * Returns true if display is on
 */
bool ILI9341::isdisplayOn() const {
    return state.disp_on_off[DISPLAY_1];
}