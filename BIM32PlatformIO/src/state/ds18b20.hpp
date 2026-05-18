#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct DS18B20State {
    float temp = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["ds18b20"]["temp"] = temp;
    }
};