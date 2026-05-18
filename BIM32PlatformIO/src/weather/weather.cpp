#include <Arduino.h>

#include "./weather.hpp"
#include "../state/state.hpp"
#include "../languages/languages.hpp"
#include "../globals.hpp"

Weather weather;

/**
 * Weather icon code conversion
 */
unsigned int Weather::_convertIcon(int code) {
    switch(code) {
        case 1: return 1; break;
        case 2: return 2; break;
        case 3: return 2; break;
        case 4: return 3; break;
        case 9: return 4; break;
        case 10: return 5; break;
        case 11: return 6; break;
        case 13: return 7; break;
        case 50: return 8; break;
        default: return 0; break;
    }
}

const char* Weather::_openMeteoCode2Description(uint8_t code) {
    switch(code) {
        case 0: return lang.weatherDescription(0);
        case 1: return lang.weatherDescription(1);
        case 2: return lang.weatherDescription(2);
        case 3:	return lang.weatherDescription(3);
        case 45: return lang.weatherDescription(4);
        case 48: return lang.weatherDescription(5);
        case 51: case 53: case 55: return lang.weatherDescription(6);
        case 56: case 57: return lang.weatherDescription(7);
        case 61: case 63: case 65: return lang.weatherDescription(8);
        case 66: case 67: return lang.weatherDescription(9);
        case 71: case 73: case 75: return lang.weatherDescription(10);
        case 77: return lang.weatherDescription(11);
        case 80: case 81: case 82: return lang.weatherDescription(12);
        case 85: case 86: return lang.weatherDescription(13);
        case 95: return lang.weatherDescription(14);
        case 96: case 99: return lang.weatherDescription(15);
        default: return "---";
    }
}

/**
 * Weatherbit icon code conversion
 */
unsigned int Weather::_weatherbitIcon(int code) {
    unsigned int icon = 4;
    if(code >= 200 and code < 300) icon = 11;
    if(code >= 300 and code < 400) icon = 9;
    if(code >= 500 and code < 600) icon = 10;
    if(code >= 600 and code < 700) icon = 13;
    if(code >= 700 and code < 800) icon = 50;
    if(code == 800) icon = 1;
    if(code == 801 or code == 802) icon = 2;
    if(code == 803) icon = 3;
    if(code >= 804) icon = 4;
    return icon;
}

/**
 * Open-meteo icon code conversion
 */
unsigned int Weather::_openMeteoIcon(int code) {
    switch(code) {
        case 0: case 1: return 1;
        case 2: return 2;
        case 3:	return 4;
        case 45: case 48: return 50;
        case 51: case 53: case 55: case 56: case 57: return 10;
        case 61: case 63: case 65: case 66: case 67: case 80: case 81: case 82: return 9;
        case 71: case 73: case 75: case 77: case 85: case 86: return 13;
        case 95: case 96: case 99: return 11;
        default: return 1;
    }
}

/**
 * Calculate daily from hourly forecast
 */
void Weather::_calculateDaily(void) {
    Serial.print("Calculate daily forecast... ");
    for(unsigned int i=0; i<DAYS; i++) {
        state.weather.daily.tMax[i] = -40400.0;
        state.weather.daily.tMin[i] = 40400.0;
        state.weather.daily.wind[i] = -1.0;
        state.weather.daily.icon[i] = 0;
    }
    for(unsigned int i=0; i<DAYS*8; i++) {
        unsigned int wd = weekday(state.weather.time);
        if(i == 0) {
            state.weather.daily.tMax[0] = state.weather.temp;
            state.weather.daily.tMin[0] = state.weather.temp;
            state.weather.daily.icon[0] = state.weather.icon;
            state.weather.daily.wind[0] = state.weather.wind.speed;
        }
        for(unsigned int k=0; k<DAYS; k++) {
            if(weekday(state.weather.hourly.date[i]) == wd) {
                if(state.weather.daily.tMax[k] < state.weather.hourly.temp[i]) state.weather.daily.tMax[k] = state.weather.hourly.temp[i];
                if(state.weather.daily.tMin[k] > state.weather.hourly.temp[i]) state.weather.daily.tMin[k] = state.weather.hourly.temp[i];
                if(state.weather.daily.icon[k] < state.weather.hourly.icon[i]) state.weather.daily.icon[k] = state.weather.hourly.icon[i];
                if(state.weather.daily.wind[k] < state.weather.hourly.windSpeed[i]) state.weather.daily.wind[k] = state.weather.hourly.windSpeed[i];
            }
            if(++wd > 7) wd = 1;
        }
    }
    Serial.println("done");
}