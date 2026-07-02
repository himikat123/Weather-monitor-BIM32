#include <Arduino.h>
#include <TimeLib.h> 

#include "./taskData.hpp"
#include "../config/config.hpp"
#include "../cloud/thingspeak/thingspeak.hpp"
#include "../network/network.hpp"

void TaskData::_updateHistoryRepository() {
    if(config.cloud.history.period() > 0) {
        if(now() - _historyUpdate > config.cloud.history.period() * 60) {
            _historyUpdate = now();
            Serial.println(SEPARATOR);
            Serial.println("Send data to weather history repository... ");
            if(network.isConnected()) thingspeak.sendHistory();
            else Serial.println("No internet connection");
            Serial.println(SEPARATOR);
            Serial.println("Receive data from weather history repository... ");
            if(network.isConnected()) thingspeak.receiveHistory();
            else Serial.println("No internet connection");
        }
    }
}