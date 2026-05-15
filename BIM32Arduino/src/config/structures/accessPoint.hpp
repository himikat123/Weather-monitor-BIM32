#pragma once
#include <Arduino.h>

class Config;

struct AccessPoint {
    private:
        char _ssid[33] = "BIM32"; // SSID
        char _pass[33] = "1234567890"; // Password
        unsigned int _chnl = 1; // WiFi channel number 1...13
        char _ip[33] = "192.168.4.1"; // IP address
        char _mask[33] = "255.255.255.0"; // Subnet mask
        friend class Config;

    public:
        const char* ssid() const { return _ssid; }
        const char* pass() const { return _pass; }
        const unsigned int chnl() const { if(_chnl < 1 or _chnl > 13) return 1; return _chnl; }
        const char* ip() const { return _ip; }
        const char* mask() const { return _mask; }
};