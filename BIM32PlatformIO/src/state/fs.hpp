#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct FilesystemState {
    unsigned int total = 0;
    unsigned int free = 0;
    char list[2048] = "";
    bool updated = false;
    bool fsInfoUpdate = true;

    void toJson(JsonObject obj) const {
        JsonObject o = obj["fs"].to<JsonObject>();
        o["total"] = total;
        o["free"] = free;
        o["list"] = list;
    }
};