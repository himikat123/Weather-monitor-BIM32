#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct MAX44009State {
    float light = -1.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        JsonObject max = o["max44009"].to<JsonObject>();
        max["light"] = light;
    }
};