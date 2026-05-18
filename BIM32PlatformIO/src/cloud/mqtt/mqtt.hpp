#pragma once

#include <PubSubClient.h> // v2.8.0 https://github.com/knolleary/pubsubclient

class MQTT {
    protected:
        PubSubClient mqttClient;
    
    public:
        MQTT() : mqttClient(client) {}
        void init();
        bool loop();
        void send();

    private:
        void _reconnect(); 
        bool _sendSensorData(unsigned int sensNum);
};