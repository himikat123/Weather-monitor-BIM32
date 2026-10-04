#include "./segmentDisplay.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"
#include "../../globals.hpp"

/**
 * Set display model
 */
void SegmentDisplay::_setModel(uint8_t model) {
    switch(model) {
        case D_WS2812_4: case D_SK9822_4: _dispLength = 4; break;
        case D_WS2812_6: case D_SK9822_6: _dispLength = 6; break;
        case D_SK9822_8: _dispLength = 8; break;
        default: _dispLength = 4; break;
    }
}

/**
 * Set time points frequency (period)
 * @param frequency (period)
 */
void SegmentDisplay::setDotFreq(uint16_t freq) {
    _dotfreq = freq;
}

/**
 * Toggle display (on/off)
 */
void SegmentDisplay::displayToggle() {
    state.disp_on_off[_dispNum] = !state.disp_on_off[_dispNum];
}

/**
 * Turn display on
 */
void SegmentDisplay::displayOn(bool doinit) {
    state.disp_on_off[_dispNum] = true;
}

/*
 * Turn display off
 */
void SegmentDisplay::displayOff() {
    state.disp_on_off[_dispNum] = false;
}

/**
 * Return true if display is on
 */
bool SegmentDisplay::isdisplayOn() const {
    return state.disp_on_off[_dispNum];
}

/**
 * Set display brightness
 * @param intensity brightness
 * @param reduc brightness is reduced or not
 */
void SegmentDisplay::brightness(unsigned int intensity, bool reduc) {
    _brightness = reduc ? round(intensity / 2) : intensity;
}