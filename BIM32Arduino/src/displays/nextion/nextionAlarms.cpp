#include <Arduino.h>
#include "./nextion.hpp"

#include "../../config/config.hpp"

/**
 * Sending alarms data
 */
void Nextion::_alarms() {
    if(_prevAlarmChecksum != _alarmChecksum or _forced) {
        unsigned int alarmOn = 0;
        char alarmData[144];
        char buf[3];
        Serial1.print("Alarm.alarms.txt=\"");
        for(uint8_t i=0; i<12; i++) {
            sprintf(buf, "%02d", config.alarm.time(i, 0));
            for(uint8_t j=0; j<2; j++) alarmData[i * 4 + j] = buf[j];
            sprintf(buf, "%02d", config.alarm.time(i, 1));
            for(uint8_t j=0; j<2; j++) alarmData[i * 4 + j + 2] = buf[j];
            for(uint8_t w=0; w<7; w++) {
                sprintf(buf, "%d", config.alarm.weekday(i, w));
                alarmData[i * 7 + w + 48] = buf[0];
            }
            sprintf(buf, "%d", config.alarm.state(i));
            alarmData[i + 132] = buf[0];
            alarmOn |= config.alarm.state(i);
        }
        for(uint8_t k=0; k<144; k++) Serial1.print(alarmData[k]);
        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        _nextion.writeNum("Alarm.alarmUpd.en", 1);
        _nextion.writeNum("Main.alarm.pic", alarmOn ? 71 : 72);

        _prevAlarmChecksum = _alarmChecksum;
    }
}