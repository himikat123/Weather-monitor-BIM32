#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct BMP180State {
    float temp = 40400.0;
    float pres = 40400.0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["bmp180"]["temp"] = temp;
        o["bmp180"]["pres"] = pres;
    }
};