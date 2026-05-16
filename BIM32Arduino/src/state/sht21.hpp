#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct SHT21State {
    float temp = 40400.0;
    float hum = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["sht21"]["temp"] = temp;
        o["sht21"]["hum"] = hum;
    }
};