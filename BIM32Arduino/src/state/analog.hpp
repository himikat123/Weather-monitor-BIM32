#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct AnalogState {
    float volt = -1.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["analog"]["volt"] = volt;
    }
};