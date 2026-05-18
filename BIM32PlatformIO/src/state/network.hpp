#pragma once

#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include "../globals.hpp"

struct NetworkState {
    bool updated = false;
    char ssid[SSID_LEN] = "";
    unsigned int ch = 0;
    int sig = 0;
    char mac[MAC_LEN] = "";
    char ip[IP_LEN] = "";
    char mask[IP_LEN] = "";
    char gw[IP_LEN] = "";
    char dns1[IP_LEN] = "";
    char dns2[IP_LEN] = "";

    unsigned int nets = 0; // Number of available networks
    char ssids[SSID_COUNT][SSID_LEN] = { 0 }; // List of available networks
    int rssis[SSID_COUNT] = { 0 }; // List of signal strengths of available networks

    void toJson(JsonObject o) const {
        o["network"]["ssid"] = ssid;
        o["network"]["ch"] = ch;
        o["network"]["sig"] = sig;
        o["network"]["mac"] = mac;
        o["network"]["ip"] = ip;
        o["network"]["mask"] = mask;
        o["network"]["gw"] = gw;
        o["network"]["dns1"] = dns1;
        o["network"]["dns2"] = dns2;

        JsonArray a_ssids = o.createNestedArray("ssids");
        for(int i=0; i<nets; i++) {
            JsonArray s = a_ssids.createNestedArray();
            s.add(ssids[i]);
            s.add(rssis[i]);
        }
    }
};