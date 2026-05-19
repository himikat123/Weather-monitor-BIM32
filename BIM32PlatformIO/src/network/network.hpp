#pragma once
#include <Arduino.h>

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

extern Network network;