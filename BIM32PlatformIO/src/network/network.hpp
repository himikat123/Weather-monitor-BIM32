#pragma once
#include <Arduino.h>

class Network {
    private:
        Network() {}

        bool _needToPing = true;
        void _connecting(uint8_t num);

    public:
        static Network& getInstance() {
            static Network instance;
            return instance;
        }

     Network(const Network&) = delete;
        void operator=(const Network&) = delete;

        void setNeedToPing();
        bool isConnected();
        void connect();
        void runAccessPoint();
        void scanNetworks();
};

inline Network& network = Network::getInstance();