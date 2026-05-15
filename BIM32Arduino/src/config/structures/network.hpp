#pragma once
#include <Arduino.h>
#include "../globals.hpp"

class Config;

struct Network {
    private:
        char _ssid[NETWORKS][33] = { "", "", "" }; // SSID list
        char _pass[NETWORKS][33] = { "", "", "" }; // Password list
        bool _type = false; // Connection type: false = dynamic IP, true = static IP
        char _ip[16] = ""; // Static IP address
        char _mask[16] = ""; // Static Subnet mask
        char _gw[16] = ""; // Static default gateway
        char _dns1[16] = ""; // Static DNS1 address
        char _dns2[16] = ""; // Static DNS2 address
        friend class Config;

    public:
        const char* ssid(unsigned int num) const { if(num >= NETWORKS) return (char*) ""; return _ssid[num]; }
        const char* pass(unsigned int num) const { if(num >= NETWORKS) return (char*) ""; return _pass[num]; }
        const bool type() const { return _type; }
        const char* ip() const { return _ip; }
        const char* mask() const { return _mask; }
        const char* gw() const { return _gw; }
        const char* dns1() const { return _dns1; }
        const char* dns2() const { return _dns2; }
};