#pragma once

#include <HTTPClient.h>
#include <PubSubClient.h> // v2.8.0 https://github.com/knolleary/pubsubclient

#include "../../agregateData/sendData/agregateSendData.hpp"

class MQTT {
    protected:
        PubSubClient mqttClient;
    
    public:
        WiFiClient client;
        MQTT() : mqttClient(client) {}
        void init();
        bool loop();
        void send();

    private:
        AgregateSendData agregateSendData;
        void _reconnect(); 
        bool _sendSensorData(unsigned int sensNum);
};