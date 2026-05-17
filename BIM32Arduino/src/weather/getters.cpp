#include <Arduino.h>

#include "./weather.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"

float Weather::get_currentTemp() {
    return state.weather.temp + config.weather.tempCorr();
}

float Weather::get_currentHum() {
    return state.weather.hum + config.weather.humCorr();
}

float Weather::get_currentPres() {
    return state.weather.pres + config.weather.presCorr();
}

float Weather::get_currentWindSpeed() {
    return state.weather.wind.speed;
}

int Weather::get_currentWindDir() {
    return state.weather.wind.dir;
}

unsigned int Weather::get_currentIcon() {
    return _convertIcon(state.weather.icon);
}

bool Weather::get_isDay() {
    return state.weather.isDay;
}

String Weather::get_description() {
    return String(state.weather.descript);
}

time_t Weather::get_currentUpdated() {
    return state.weather.time;
}

float Weather::get_dailyDayTemp(unsigned int num) {
    if(num >= DAYS) return UNDEFINED_FLOAT;
    return state.weather.daily.tMax[num];
}

float Weather::get_dailyNightTemp(unsigned int num) {
    if(num >= DAYS) return UNDEFINED_FLOAT;
    return state.weather.daily.tMin[num];
}

float Weather::get_dailyWindSpeed(unsigned int num) {
    if(num >= DAYS) return UNDEFINED_FLOAT;
    return state.weather.daily.wind[num];
}

unsigned int Weather::get_dailyIcon(unsigned int num) {
    if(num >= DAYS) return 0;
    return _convertIcon(state.weather.daily.icon[num]);
}

unsigned int Weather::get_hourlyDate(unsigned int num) {
    if(num >= DAYS * 8) return 0;
    return state.weather.hourly.date[num];
}

unsigned int Weather::get_hourlyIcon(unsigned int num) {
    if(num >= DAYS * 8) return 0;
    return state.weather.hourly.icon[num];
}

float Weather::get_hourlyTemp(unsigned int num) {
    if(num >= DAYS * 8) return UNDEFINED_FLOAT;
    return state.weather.hourly.temp[num];
}

float Weather::get_hourlyPres(unsigned int num) {
    if(num >= DAYS * 8) return UNDEFINED_FLOAT;
    return state.weather.hourly.pres[num];
}

float Weather::get_hourlyWindSpeed(unsigned int num) {
    if(num >= DAYS * 8) return UNDEFINED_FLOAT;
    return state.weather.hourly.windSpeed[num];
}

int Weather::get_hourlyWindDir(unsigned int num) {
    if(num >= DAYS * 8) return -1;
    return state.weather.hourly.windDir[num];
}

float Weather::get_hourlyPrec(unsigned int num) {
    if(num >= DAYS * 8) return UNDEFINED_FLOAT;
    return state.weather.hourly.prec[num];
}