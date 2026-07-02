#include <Arduino.h> 

#include "./taskData.hpp"
#include "../globals.hpp"
#include "../config/config.hpp"
#include "../network/network.hpp"

void TaskData::_sendNarodmon() {
    if(config.cloud.narodmonSend.period() > 0) {
        if((millis() - _narodmonSend) > (config.cloud.narodmonSend.period() * 60000) or _narodmonSend == 0) {
            _narodmonSend = millis();
            Serial.println(SEPARATOR);
            Serial.println("Send data to narodmon.ru... ");
            if(network.isConnected()) narodmon.send();
            else {
                _narodmonSend = 0;
                Serial.println("No internet connection");
            }
        }
    }
}