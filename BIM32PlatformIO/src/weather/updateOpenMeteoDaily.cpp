#include <Arduino.h>
#include <HTTPClient.h>
#include <JSON_Decoder.h> // https://github.com/Bodmer/JSON_Decoder

#include "./weather.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"

/**
 * Update daily forecast from open-meteo.com
 */
void Weather::_updateOpenMeteoDaily() {
    Serial.println(SEPARATOR);
    Serial.println("Open-meteo.com: daily forecast update... ");
  
    String url = "http://api.open-meteo.com/v1/forecast";
    url += "?latitude=" + String(config.weather.lat());
    url += "&longitude=" + String(config.weather.lon());
    url += "&daily=weather_code,temperature_2m_max,temperature_2m_min,wind_speed_10m_max";
    url += "&wind_speed_unit=ms&timeformat=unixtime&timezone=auto&forecast_days=4";
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
            Serial.println("Open-meteo.com: daily forecast deserialization error");
            return;
        }
        for(unsigned int i=0; i<DAYS; i++) {
            state.weather.daily.tMax[i] = forecast["daily"]["temperature_2m_max"][i] | 40400.0;
            state.weather.daily.tMin[i] = forecast["daily"]["temperature_2m_min"][i] | 40400.0;
            state.weather.daily.wind[i] = forecast["daily"]["wind_speed_10m_max"][i] | -1.0;
            state.weather.daily.icon[i] = _openMeteoIcon(forecast["daily"]["weather_code"][i] | 0);
        }
        httpData = "";
        state.weather.daily.time = now();
        Serial.print("Open-meteo.com: daily forecast updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    }
    else Serial.println("Open-meteo.com: daily forecast update error");
    clientDaily.end();
}