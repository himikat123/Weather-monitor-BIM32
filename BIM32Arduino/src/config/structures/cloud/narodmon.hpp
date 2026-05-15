#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"
#include "./base.hpp"

class Config;

struct NarodmonSend : public NarodmonMqttBase {
    private:
        char _lon[11] = ""; // Longitude for Narodmon
        char _lat[11] = ""; // Latitude for Narodmon
        char _name[17] = "BIM"; // Sensor name
        char _metrics[NAROD_FIELDS][17] { 0 }; // Sensor metrics to send to Narodmon fields
        friend class Config;

    public:
        const char* lon() const { return _lon; }
        const char* lat() const { return _lat; }
        const char* name() const { return _name; }
        const char* metrics(unsigned int num) const { return (num >= NAROD_FIELDS) ? (char*) "" : _metrics[num]; }
};