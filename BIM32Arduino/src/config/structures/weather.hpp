#pragma once
#include <Arduino.h>
#include "../globals.hpp"

class Config;

struct Weather {
    private:
        char _appid[PROVIDERS][33] = { "", "" }; // [0] -> APPID openweathermap.org, [1] -> KEY weatherbit.io
        float _lon = 0.0; // Longitude
        float _lat = 0.0; // Latitude
        unsigned int _provider = 0; // Weather forecast provider. 0: openweathermap.org, 1: weatherbit.io, 2: open-meteo.com 
        char _city[41] = ""; // City name
        unsigned int _cityid = 0; // City ID
        unsigned int _citysearch = 0; // The way to recognize a city. 0 = by name, 1 = by ID, 2 = by coordinates
        float _tempCorr = 0; // Weather temperature correction
        float _humCorr = 0; // Weather humidity correction
        float _presCorr = 0; // Weather pressure correction
        friend class Config;

    public:
        const char* appid(unsigned int num) const { if(num >= PROVIDERS) return (char*) ""; return _appid[num]; }
        unsigned int provider() const { if(_provider > 2) return 0; return _provider; }
        unsigned int citysearch() const { if(_citysearch > 2) return 0; return _citysearch; }
        const char* city() const { return _city; }
        unsigned int cityid() const { return _cityid; }
        float lon() const { return _lon; }
        float lat() const { return _lat; }
        float tempCorr() const { return _tempCorr; }
        float humCorr() const { return _humCorr; }
        float presCorr() const { return _presCorr; }
};