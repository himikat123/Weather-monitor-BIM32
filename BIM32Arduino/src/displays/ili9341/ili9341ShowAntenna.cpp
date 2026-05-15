#include "./ili9341.hpp"

#include "./pictures/ants/ant0.hpp"
#include "./pictures/ants/ant1.hpp"
#include "./pictures/ants/ant2.hpp"
#include "./pictures/ants/ant3.hpp"
#include "./pictures/ants/ant4.hpp"
#include "./pictures/ants/acpoint.hpp"

/**
 * Display antenna symbol
 */
void ILI9341::_showAntenna() {
    if(_prevRssi != _rssi || _prevIsApMode != _isApMode || _forced) {
        if(_isApMode) _showImg(292, 1, ant_acpoint, sizeof(ant_acpoint));
        else {
            if(_rssi > -51) _showImg(292, 1, ant4, sizeof(ant4));
            if(_rssi < -50 && _rssi > -76) _showImg(292, 1, ant3, sizeof(ant3));
            if(_rssi <- 75 && _rssi > -96) _showImg(292, 1, ant2, sizeof(ant2));
            if(_rssi < -95) _showImg(292, 1, ant1, sizeof(ant1));
            if(_rssi >= 0) _showImg(292, 1, ant0, sizeof(ant0));
        }
        _prevRssi = _rssi;
        _prevIsApMode = _isApMode;
    }
}