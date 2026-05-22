#include <Arduino.h>

#include "./taskSensors.hpp"
#include "../config/config.hpp"
#include "../network/network.hpp"

void TaskSensors::_sendMqtt() {
    if(config.cloud.mqttSend.period() > 0 and network.isConnected()) {
        if(mqtt.loop()) {
            if((millis() - _mqttSend) > (config.cloud.mqttSend.period() * 1000) or _mqttSend == 0) {
                _mqttSend = millis();
                Serial.println(SEPARATOR);
                Serial.println("Send data via MQTT...");
                if(network.isConnected()) mqtt.send();
                else {
                    _mqttSend = 0;
                    Serial.println("No internet connection");
                }
            }
        }
    }
}