#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

struct BME680State {
    float temp = 40400.0;
    float hum = 40400.0;
    float pres = 40400.0;
    float iaq = -1.0;
    unsigned int iaqAccr = 0;
    bool updated = false;

    void toJson(JsonObject o) const {
        o["bme680"]["temp"] = temp;
        o["bme680"]["hum"] = hum;
        o["bme680"]["pres"] = pres;
        o["bme680"]["iaq"] = iaq;
        o["bme680"]["iaqAccr"] = iaqAccr;
    }
};