#pragma once

#include <ESP32Ping.h> // v1.6 https://github.com/marian-craciunescu/ESP32Ping

class Network {
    private:
        bool _needToPing = true;
        void _connecting(uint8_t num);

    public:
        void setNeedToPing();
        bool isConnected();
        void connect();
        void runAccessPoint();
        void scanNetworks();
};