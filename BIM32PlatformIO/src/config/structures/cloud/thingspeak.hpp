#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"

class Config;

struct ThingBase {
    private:
        unsigned int _period = 0; // Period data update (minutes) 0...999
        char _channelID[11] = ""; // channel ID
        char _rdkey[17] = ""; // Read API Key
        friend class Config;

    public:
        const unsigned int period() const { return _period; }
        const char* channelID() const { return _channelID; }
        const char* rdkey() const { return _rdkey; }
};

struct ThinspeakReceiveStruct : public ThingBase {
    private: 
        unsigned int _expire = 10; friend class Config; // Thingspeak data expire (minutes) 1...100

    public: 
        const unsigned int expire() const { return _expire; }
};

struct ThingspeakSendStruct : public ThingBase {
    private:
        char _wrkey[17] = ""; // Write API Key
        unsigned int _fields[THNG_FIELDS] = { 0 }; // Data sources
        unsigned int _types[THNG_FIELDS] = { 0 };
        unsigned int _wsensors[THNG_FIELDS] = { 0 }; // Wireless sensor numbers
        unsigned int _wtypes[THNG_FIELDS] = { 0 }; // Wireless sensor data types
        friend class Config;

    public:
        const char* wrkey() const { return _wrkey; }
        const unsigned int fields(unsigned int num) const { return (num >= THNG_FIELDS) ? 0 : _fields[num]; }
        const unsigned int types(unsigned int num) const { return (num >= THNG_FIELDS) ? 0 : _types[num]; }
        const unsigned int wsensors(unsigned int num) const { return (num >= THNG_FIELDS or _wsensors[num] >= WSENSORS) ? 0 : _wsensors[num]; }
        const unsigned int wtypes(unsigned int num) const { return (num >= THNG_FIELDS) ? 0 : _wtypes[num]; }
};