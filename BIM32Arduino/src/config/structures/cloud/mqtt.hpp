#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"
#include "./base.hpp"

class Config;

struct MQTTSend : public NarodmonMqttBase{
    private:
        char _broker[33] = ""; // MQTT broker address
        unsigned int _port = 1883; // MQTT port
        char _user[33] = ""; // MQTT username
        char _pass[33] = ""; // MQTT password
        char _topics[12][17] = { 0 }; // Topics for sending via MQTT
        friend class Config;

    public:
        const char* broker() const { return _broker; }
        const unsigned int port() const { return (_port > 65535) ? 1883 : _port; }
        const char* user() const { return _user; }
        const char* pass() const { return _pass; }
        const char* topics(unsigned int num) const { return (num >= MQTT_TOPICS) ? (char*) "" : _topics[num]; }
};