#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct NarodmonMqttBase {
    private:
        unsigned int _period = 0; // Period for data sending (minutes) 0...999
        unsigned int _sensors[NAROD_FIELDS] = { 0 }; // Data sources to send
        unsigned int _types[NAROD_FIELDS] = { 0 }; // Wired sensor data types to send
        unsigned int _wsensors[NAROD_FIELDS] = { 0 }; // Wireless sensor numbers
        unsigned int _wtypes[NAROD_FIELDS] = { 0 }; // Wireless sensor data types
        friend class Config;

    public:
        const unsigned int period() const { return (_period > 999) ? 5 : _period; }
        const unsigned int sensors(unsigned int num) const { return (num >= NAROD_FIELDS) ? 0 : _sensors[num]; }
        const unsigned int types(unsigned int num) const { return (num >= NAROD_FIELDS) ? 0 : _types[num]; }
        const unsigned int wsensors(unsigned int num) const { return (num >= NAROD_FIELDS or _wsensors[num] >= WSENSORS) ? 0 : _wsensors[num]; }
        const unsigned int wtypes(unsigned int num) const { return (num >= NAROD_FIELDS) ? 0 : _wtypes[num]; }
};