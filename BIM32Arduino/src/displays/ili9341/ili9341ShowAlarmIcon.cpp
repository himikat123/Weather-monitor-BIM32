#include "./ili9341.hpp"

#include "../../config/config.hpp"
#include "./pictures/symbols/alarm.hpp"
#include "./pictures/symbols/alarmOff.hpp"

/**
 * Display Alarm icon
 */
void ILI9341::_showAlarmIcon() {
    uint8_t alarmOn = 0;
    for(uint8_t i=0; i<12; i++) {
        alarmOn |= config.alarm.state(i);
    }
    if(_prevAlarmOn != alarmOn || _forced) {
        if(alarmOn) _showImg(294, 140, symb_alarm, sizeof(symb_alarm));
        else _showImg(294, 140, symb_alarm_off, sizeof(symb_alarm_off));
        _prevAlarmOn = alarmOn;
    }
}