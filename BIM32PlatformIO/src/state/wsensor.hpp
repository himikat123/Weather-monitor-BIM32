#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include "../globals.hpp"

struct WSensorState {
    time_t time[2] = { 0 };
    bool updated = false;

    struct Block1D {
        float data[2] = { UNDEFINED_FLOAT, UNDEFINED_FLOAT };
        char name[2][W_NAME_LEN] = { 0 };

        void toJson(JsonObject o) const {
            JsonArray D = o["data"].to<JsonArray>();
            JsonArray N = o["name"].to<JsonArray>();
            for(int i=0; i<2; i++) {
                D.add(data[i]);
                N.add(name[i]);
            }
        }
    };

    struct Block2D {
        float data[5][2] = { 
            {UNDEFINED_FLOAT, UNDEFINED_FLOAT}, 
            {UNDEFINED_FLOAT, UNDEFINED_FLOAT}, 
            {UNDEFINED_FLOAT, UNDEFINED_FLOAT}, 
            {UNDEFINED_FLOAT, UNDEFINED_FLOAT}, 
            {UNDEFINED_FLOAT, UNDEFINED_FLOAT} 
        };
        char name[5][2][W_NAME_LEN] = { 0 };

        void toJson(JsonObject o) const {
            JsonArray dataArr = o["data"].to<JsonArray>();
            JsonArray nameArr = o["name"].to<JsonArray>();
            for(int r=0; r<5; r++){
                JsonArray rowD = dataArr.add<JsonArray>();
                JsonArray rowN = nameArr.add<JsonArray>();
                for(int c=0; c<2; c++){
                    rowD.add(data[r][c]);
                    rowN.add(name[r][c]);
                }
            }
        }
    };

    struct BlockWind {
        Block1D speed, dir;

        void toJson(JsonObject o) const {
            speed.toJson(o["speed"].to<JsonObject>());
            dir.toJson(o["dir"].to<JsonObject>());
        }
    };

    struct BlockBattery {
        int adc[2] = {-1, -1};
        int level[2] = {-1, -1};
        float voltage[2] = {UNDEFINED_FLOAT, UNDEFINED_FLOAT};
        float percentage[2] = {UNDEFINED_FLOAT, UNDEFINED_FLOAT};
    };

    BlockBattery battery;
    BlockWind wind;
    Block2D temp;
    Block1D hum, pres, light, voltage, current, power, energy, freq, co2;

    void toJson(JsonObject obj) const {
        JsonObject o = obj["wsensor"].to<JsonObject>();
        JsonArray aTime = o["time"].to<JsonArray>();
        aTime.add(time[0]);
        aTime.add(time[1]);

        temp.toJson(o["temp"].to<JsonObject>());
        hum.toJson(o["hum"].to<JsonObject>());
        pres.toJson(o["pres"].to<JsonObject>());
        light.toJson(o["light"].to<JsonObject>());
        voltage.toJson(o["voltage"].to<JsonObject>());
        current.toJson(o["current"].to<JsonObject>());
        power.toJson(o["power"].to<JsonObject>());
        energy.toJson(o["energy"].to<JsonObject>());
        freq.toJson(o["freq"].to<JsonObject>());
        co2.toJson(o["co2"].to<JsonObject>()); 
        wind.toJson(o["wind"].to<JsonObject>());

        JsonArray b = o["bat"].to<JsonArray>();
        for(int i=0; i<2; i++) b.add(battery.adc[i]);
    }
};