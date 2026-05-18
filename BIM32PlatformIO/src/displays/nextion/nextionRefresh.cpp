#include <Arduino.h>
#include "./nextion.hpp"
#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

void Nextion::refresh() {
    if(millis() - _prevForced > 10000) {
        _forced = true;
        _prevForced = millis();
        _nextion.writeNum("BigClock.clockFormat.val", config.clock.format());
    }

    if(_prevPower != _power or _forced) {
        _nextion.writeNum("sleep", _power ? 0 : 1);
        _prevPower = _power;
    }

    if(_power) {
        _getData();

        if(config.display.model(DISPLAY_1) == D_NX4832T035) {
            _NX4832T035_timeDate();
        }
        if(config.display.model(DISPLAY_1) == D_NX4832K035 or config.display.model(DISPLAY_1) == D_NX4827K043) {
            if(state.clockSynchronize) {
                _setRTC();
                state.clockSynchronize = false;
            }
        }

        _networkPage();
        _showComfortLevel();
        _showVoltage();
        _showBattery();
        _showAntenna();

        _showTempIn();
        _showHumIn();
        _showSequence();
  
        _showWeatherDescription();
        _showCurrentIcon();
        _showTempOut();
        _showHumOut();
        _showPres();
        _showWindSpeed();
        _showWindDirection();
        _showUpdated();
        _showWeatherForecast();
        _hourlyData();
        _historyOut();
        _historyIn();
        _alarms();
    }
}