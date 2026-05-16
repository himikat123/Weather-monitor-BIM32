#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct ThingState {
    time_t time = 0;
    unsigned int historyTime[24] = { 0 };
    float data[8] = { -40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0, -40400.0 };
    float historyData[7][24] = { 0 };
    bool updated = false;

    void toJson(JsonObject obj) const {
        JsonObject o = obj.createNestedObject("thing");
        o["time"] = time;
        JsonArray a = o.createNestedArray("data");
        for(int i=0;i<8;i++) a.add(data[i]);
    }
};