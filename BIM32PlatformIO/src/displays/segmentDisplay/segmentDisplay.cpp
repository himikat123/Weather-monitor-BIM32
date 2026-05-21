#include "./segmentDisplay.hpp"
#include "../../config/config.hpp"

/**
 * Set display model
 */
void SegmentDisplay::_setModel(uint8_t model) {
    switch(config.display.type(_dispNum)) {
        case 2: _dispLength = model < 3 ? 4 : 6; break;
        case 3: _dispLength = (model == 0 || model == 2) ? 4 : (model == 1 || model == 3) ? 6 : 8; break;
        case 4: _dispLength = model == 0 ? 4 : model == 1 ? 6 : 8; break;
        default: _dispLength = 4;
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
    _power = !_power;
}

/**
 * Turn display on
 */
void SegmentDisplay::displayOn(bool doinit) {
    _power = true;
}

/*
 * Turn display off
 */
void SegmentDisplay::displayOff() {
    _power = false;
}

/**
 * Return true if display is on
 */
bool SegmentDisplay::isdisplayOn() const {
    return _power;
}

/**
 * Set display brightness
 * @param intensity brightness
 * @param reduc brightness is reduced or not
 */
void SegmentDisplay::brightness(unsigned int intensity, bool reduc) {
    _brightness = reduc ? round(intensity / 2) : intensity;
}