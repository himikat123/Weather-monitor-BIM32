#include <Arduino.h>
#include <JSON_Decoder.h> // https://github.com/Bodmer/JSON_Decoder
#include <TimeLib.h>

#include "./weather.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"

/**
 * Update daily forecast from weatherbit.io
 */
void Weather::_updateWeatherbitDaily(void) {
    Serial.println(SEPARATOR);
    Serial.println("Weatherbit.io: daily forecast update... ");
  
    String url = "http://api.weatherbit.io/v2.0/forecast/daily?days=5";
    if(config.weather.citysearch() == 0) url += "&city=" + String(config.weather.city());
    if(config.weather.citysearch() == 1) url += "&city_id=" + String(config.weather.cityid());
    if(config.weather.citysearch() == 2) url += "&lat=" + String(config.weather.lat()) + "&lon=" + String(config.weather.lon());
    url += "&key=" + String(config.weather.appid(WEATHERBIT));
    HTTPClient clientDaily;
    if(state.debugWether) {
        Serial.println(url);
    }
    clientDaily.begin(url);
    int httpCode = clientDaily.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientDaily.getString();
        if(state.debugWether) {
            Serial.println(httpData);
        }
        JsonDocument forecast;
        DeserializationError errorForecast = deserializeJson(forecast, httpData);
        if(errorForecast) {
            Serial.println("Weatherbit.io: daily forecast deserialization error");
            return;
        }
        for(unsigned int i=0; i<DAYS; i++) {
            state.weather.daily.tMax[i] = forecast["data"][i]["high_temp"] | 40400.0;
            state.weather.daily.tMin[i] = forecast["data"][i]["min_temp"] | 40400.0;
            state.weather.daily.wind[i] = forecast["data"][i]["wind_spd"] | -1.0;
            state.weather.daily.icon[i] = _weatherbitIcon(forecast["data"][i]["weather"]["code"].as<int>() | 0);
        }
        httpData = "";
        state.weather.daily.time = now();
        Serial.print("Weatherbit.io: daily forecast updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    }
    else Serial.println("Weatherbit.io: daily forecast update error");
    clientDaily.end();
}