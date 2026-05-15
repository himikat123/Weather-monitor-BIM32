#include "./nextion.hpp"

/**
 * Display antenna symbol
 */
void Nextion::_showAntenna() {
    if(_prevRssi != _rssi or _prevIsApMode != _isApMode or _forced) {
        uint8_t ant = 0;
        if(_isApMode) ant = 70;
        else {
            ant = 30;
            if(_rssi > -51) ant = 34;
            if(_rssi < -50 && _rssi > -76) ant = 33;
            if(_rssi < -75 && _rssi > -96) ant = 32;
            if(_rssi < -95) ant = 31;
            if(_rssi >= 0) ant = 30;
        }
        _prevRssi = _rssi;
        _prevIsApMode = _isApMode;
        _nextion.writeNum("Main.ant.pic", ant);
    }
}