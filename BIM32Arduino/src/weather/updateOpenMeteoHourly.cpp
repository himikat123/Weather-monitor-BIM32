#include <Arduino.h>
#include <HTTPClient.h>
#include <JSON_Decoder.h> // https://github.com/Bodmer/JSON_Decoder

#include "./weather.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"

/**
 * Update hourly forecast from open-meteo.com
 */
void Weather::_updateOpenMeteoHourly() {
    Serial.println(SEPARATOR);
    Serial.println("Open-meteo.com: hourly forecast update... ");
  
    String url = "http://api.open-meteo.com/v1/forecast";
    url += "?latitude=" + String(config.weather.lat());
    url += "&longitude=" + String(config.weather.lon());
    url += "&hourly=temperature_2m,precipitation_probability,weather_code,surface_pressure,wind_speed_10m,wind_direction_10m";
    url += "&wind_speed_unit=ms&timeformat=unixtime&timezone=auto&forecast_days=6";
    HTTPClient clientHourly;
    if(state.debugWether) {
        Serial.println(url);
    }
    clientHourly.begin(url);
    int httpCode = clientHourly.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientHourly.getString();
        if(state.debugWether) {
            Serial.println(httpData);
        }
        JsonDocument filter[7];
        filter[0]["utc_offset_seconds"] = true;
        filter[0]["hourly"]["time"] = true;
        filter[1]["hourly"]["temperature_2m"] = true;
        filter[2]["hourly"]["precipitation_probability"] = true;
        filter[3]["hourly"]["weather_code"] = true;
        filter[4]["hourly"]["surface_pressure"] = true;
        filter[5]["hourly"]["wind_speed_10m"] = true;
        filter[6]["hourly"]["wind_direction_10m"] = true;
        uint8_t timePoint[48];
        for(uint8_t f=0; f<7; f++) {
            JsonDocument forecast;
            DeserializationError errorForecast = deserializeJson(forecast, httpData, DeserializationOption::Filter(filter[f]));
            if(errorForecast) {
                Serial.print("Open-meteo.com: hourly forecast " + String(f) + " deserialization error: ");
                return;
            }
            uint8_t n = 0;
            for(uint8_t i=0; i<144; i++) {
                if(f == 0) {
                    time_t time = forecast["hourly"]["time"][i] | 0;
                    time_t utc = forecast["utc_offset_seconds"] | 0;
                    uint8_t hr = hour(time + utc);
                    if(hr % 3 == 0 && (time + utc) >= now()) {
                        state.weather.hourly.date[n] = forecast["hourly"]["time"][i] | 0;
                        state.weather.hourly.date[n] += utc;
                        timePoint[n] = i;
                        if(n<39) n++;
                        else break;
                    }
                }
                else if(timePoint[n] == i) {
                    if(f == 1) state.weather.hourly.temp[n] = forecast["hourly"]["temperature_2m"][i] | 40400.0;
                    if(f == 4) state.weather.hourly.pres[n] = forecast["hourly"]["surface_pressure"][i] | 40400.0;
                    if(f == 5) state.weather.hourly.windSpeed[n] = forecast["hourly"]["wind_speed_10m"][i] | -1.0;
                    if(f == 6) state.weather.hourly.windDir[n] = forecast["hourly"]["wind_direction_10m"][i] | 0.0;
                    if(f == 2) state.weather.hourly.prec[n] = forecast["hourly"]["precipitation_probability"][i] | 0.0;
                    if(f == 3) state.weather.hourly.icon[n] = _openMeteoIcon(forecast["hourly"]["weather_code"][i] | 0);
                    if(n<39) n++;
                    else break;
                }  
            }
        }
        
        httpData = "";
        state.weather.hourly.time = now();
        Serial.print("Open-meteo.com: hourly forecast updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    }
    else Serial.println("Open-meteo.com: hourly forecast update error");
    clientHourly.end();
}