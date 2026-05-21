#include <Arduino.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "./nextion.hpp"

#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

/**
 * Receive date and time from display
 */
void Nextion::dataReceive() {
    while(Serial1.available()) {
        char nextionChar = Serial1.read();
        if(nextionChar == '{' or nextionChar == '\x87') {
            state.disp_autoOff[0] = millis();
            if(!isDisplayOn()) displayOn(true);
        }
        if(nextionChar == '{') _customData = 1;
        if(nextionChar == '}') {
            _customData = 0;
            _receivedData.concat(nextionChar);
        }
        if(_customData == 1) _receivedData.concat(nextionChar);
        if(_customData == 0) {
            JsonDocument root;
            DeserializationError error = deserializeJson(root, _receivedData);
            if(!error) {
                /* Time from display */
                if(_receivedData.lastIndexOf("{\"h\":") != -1) {
                    setTime(
                        root["h"].as<int>(), 
                        root["i"].as<int>(), 
                        root["s"].as<int>(), 
                        root["d"].as<int>(), 
                        root["m"].as<int>(), 
                        root["y"].as<int>()
                    );
                }

                /* Alarm settings from display */
                if(_receivedData.lastIndexOf("{\"alarms\":") != -1) {
                    uint8_t times[ALARMS][2], weekdays[ALARMS][7], states[ALARMS]; 
                    for(uint8_t i=0; i<ALARMS; i++) {
                        for(uint8_t n=0; n<10; n++) {
                            uint8_t val = root["alarms"][i][n].as<int>();
                            if(n == 0) config.alarm.setState(i, val);
                            if(n == 1) config.alarm.setTime(i, 0, val);
                            if(n == 2) config.alarm.setTime(i, 1, val);
                            if(n >= 3) config.alarm.setWeekday(i, n - 3, val);
                        }
                    }
                    _forced = true;
                    config.saveAlarmFile();
                }
            }
            _receivedData = "";
            _customData = -1;
        }
    }
}