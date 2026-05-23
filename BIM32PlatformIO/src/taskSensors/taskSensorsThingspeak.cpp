#include <Arduino.h> 

#include "./taskSensors.hpp"
#include "../globals.hpp"
#include "../config/config.hpp"
#include "../network/network.hpp"
#include "../cloud/thingspeak/thingspeak.hpp"

/**
 * Receive data from thingspeak
 */
void TaskSensors::_receiveThingspeak() {
    if(config.cloud.thingspeakReceive.period() > 0) {
        if((millis() - _thingspeakReceive) > (config.cloud.thingspeakReceive.period() * 60000) or _thingspeakReceive == 0) {
            _thingspeakReceive = millis();
            Serial.println(SEPARATOR);
            Serial.println("Receive data from thingspeak.com... ");
            if(network.isConnected()) thingspeak.receive();
            else {
                _thingspeakReceive = 0;
                Serial.println("No internet connection");
            }
        }
    }
}

/**
 * Send data to thingspeak
 */
void TaskSensors::_sendThingspeak() {
    if(config.cloud.thingspeakSend.period() > 0) {
        if((millis() - _thingspeakSend) > (config.cloud.thingspeakSend.period() * 60000) or _thingspeakSend == 0) {
            _thingspeakSend = millis();
            Serial.println(SEPARATOR);
            Serial.println("Send data to thingspeak.com... ");
            if(network.isConnected()) thingspeak.send();
            else {
                _thingspeakSend = 0;
                Serial.println("No internet connection");
            }
        }
    }
}