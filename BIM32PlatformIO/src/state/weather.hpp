#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include "../globals.hpp"

struct WeatherState {
    float temp = 40400.0;
    float hum = 40400.0;
    float pres = 40400.0;
    unsigned int icon = 1;
    bool isDay = true;
    char descript[128] = "----";
    time_t time = 0;
    bool updated = false;

    struct Wind {
        float speed = -1.0;
        int dir = 0;

        void toJson(JsonObject o) const {
            o["speed"] = speed;
            o["dir"] = dir;
        }
    } wind;

    struct Daily {
        float tMax[5] = { 40400.0, 40400.0, 40400.0, 40400.0, 40400.0 };
        float tMin[5] = { 40400.0, 40400.0, 40400.0, 40400.0, 40400.0 };
        float wind[5] = { -1.0, -1.0, -1.0, -1.0, -1.0 };
        unsigned int icon[5] = { 0, 0, 0, 0, 0 };
        time_t time = 0;

        void toJson(JsonObject o) const {
            JsonArray a_tMax = o["tMax"].to<JsonArray>();
            JsonArray a_tMin = o["tMin"].to<JsonArray>();
            JsonArray a_wind = o["wind"].to<JsonArray>();
            JsonArray a_icon = o["icon"].to<JsonArray>();
            
            for(int i=0; i<5; i++) {
                a_tMax.add(tMax[i]);
                a_tMin.add(tMin[i]);
                a_wind.add(wind[i]);
                a_icon.add(icon[i]);
            }
        }
    } daily;

    struct Hourly {
        time_t date[HOURLY_COUNT] = { 0 };
        unsigned int icon[HOURLY_COUNT] = { 0 };
        float temp[HOURLY_COUNT] = { 0 };
        float pres[HOURLY_COUNT] = { 0 };
        float windSpeed[HOURLY_COUNT] = { 0 };
        int windDir[HOURLY_COUNT] = { 0 };
        float prec[HOURLY_COUNT] = { 0 };
        time_t time = 0;

        void toJson(JsonObject o) const {
            JsonArray a_date = o["date"].to<JsonArray>();
            JsonArray a_icon = o["icon"].to<JsonArray>();
            JsonArray a_temp = o["temp"].to<JsonArray>();
            JsonArray a_pres = o["pres"].to<JsonArray>();
            JsonArray a_windSpeed = o["windSpeed"].to<JsonArray>();
            JsonArray a_windDir = o["windDir"].to<JsonArray>();
            JsonArray a_prec = o["prec"].to<JsonArray>();
            
            for(int i=0; i<HOURLY_COUNT; i++) {
                a_date.add(date[i]);
                a_icon.add(icon[i]);
                a_temp.add(temp[i]);
                a_pres.add(pres[i]);
                a_windSpeed.add(windSpeed[i]);
                a_windDir.add(windDir[i]);
                a_prec.add(prec[i]);
            }
        }
    } hourly;

    void toJson(JsonObject obj) const {
        JsonObject o = obj["weather"].to<JsonObject>();
        o["temp"] = temp;
        o["hum"] = hum;
        o["pres"] = pres;
        o["icon"] = icon;
        o["isDay"] = isDay;
        o["descript"] = descript;
        o["time"] = time;

        JsonObject windObj = o["wind"].to<JsonObject>();
        wind.toJson(windObj);

        JsonObject dailyObj = o["daily"].to<JsonObject>();
        daily.toJson(dailyObj);

        JsonObject hourlyObj = o["hourly"].to<JsonObject>();
        hourly.toJson(hourlyObj);
    }
};