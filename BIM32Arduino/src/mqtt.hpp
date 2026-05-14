#pragma once

#include <PubSubClient.h> // v2.8.0 https://github.com/knolleary/pubsubclient
PubSubClient mqttClient(client);

class MQTT {
    public:
        void init();
        bool loop();
        void send();
    private:
        void _reconnect(); 
        bool _sendSensorData(unsigned int sensNum);
};