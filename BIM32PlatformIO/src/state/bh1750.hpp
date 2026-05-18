#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct BH1750State {
    float light = -1.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["bh1750"]["light"] = light;
    }
};