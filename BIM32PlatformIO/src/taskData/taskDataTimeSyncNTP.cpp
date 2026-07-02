#include <Arduino.h> 

#include "./taskData.hpp"
#include "../globals.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../network/network.hpp"

/**
 * Time synchronization with NTP server
 */
void TaskData::_timeSyncNTP() {
    if(config.clock.ntpPeriod() > 0) {
        if((millis() - _ntp_update) > config.clock.ntpPeriod() * 60000 or !state.clockSynchronized) {
            _ntp_update = millis();
            Serial.println(SEPARATOR);
            Serial.println("NTP synchronization... ");
            if(network.isConnected()) timeNTP.syncNTP();
            else {
                state.clockSynchronized = false;
                Serial.println("No internet connection");
            }
            vTaskDelay(100);
        }
    }
}