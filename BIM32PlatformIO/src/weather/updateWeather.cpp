#include <Arduino.h>
#include <JSON_Decoder.h> // https://github.com/Bodmer/JSON_Decoder
#include <TimeLib.h>
#include <HTTPClient.h>

#include "./weather.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"

/**
 * Weather update
 */
void Weather::update() {
    if(config.weather.provider() == GISMETEO) {
        _updateGismeteo();
    }
    else {
        String url = "";
        if(config.weather.provider() == OPENWEATHERMAP) {
            String appid = String(config.weather.appid(OPENWEATHERMAP));
            if(appid == "") {
                Serial.println("No APPID");
                return;
            }
            url = "http://api.openweathermap.org/data/2.5/weather?appid=" + appid;
            if(config.weather.citysearch() == 0) {
                String city = String(config.weather.city());
                if(city == "") {
                    Serial.println("No Cityname");
                    return;
                }
                url += "&q=" + city;
            }
            if(config.weather.citysearch() == 1) {
                url += "&id=" + String(config.weather.cityid());
            }
            if(config.weather.citysearch() == 2) {
                url += "&lat=" + String(config.weather.lat()) + "&lon=" + String(config.weather.lon());
            }
            url += "&units=metric&lang=" + config.lang();
            if(state.debugWether) {
                Serial.println("OPENWEATHERMAP");
                Serial.println(url);
            }
        }

        else if(config.weather.provider() == WEATHERBIT) {
            url = "http://api.weatherbit.io/v2.0/current?key=" + String(config.weather.appid(WEATHERBIT));
            if(config.weather.citysearch() == 0) {
                String city = String(config.weather.city());
                if(city == "") {
                    Serial.println("No Cityname");
                    return;
                }
                url += "&city=" + city;
            }
            if(config.weather.citysearch() == 1) {
                url += "&city_id=" + String(config.weather.cityid());
            }
            if(config.weather.citysearch() == 2) {
                url += "&lat=" + String(config.weather.lat()) + "&lon=" + String(config.weather.lon());
            }
            url += "&lang=" + config.lang();
            if(state.debugWether) {
                Serial.println("WEATHERBIT");
                Serial.println(url);
            }
        }

        else if(config.weather.provider() == OPEN_METEO) {
            if(config.weather.citysearch() == 2) {
                url = "http://api.open-meteo.com/v1/forecast";
                url += "?latitude=" + String(config.weather.lat());
                url += "&longitude=" + String(config.weather.lon());
                url += "&current=temperature_2m,relative_humidity_2m,is_day,weather_code,pressure_msl,wind_speed_10m,wind_direction_10m";
                url += "&wind_speed_unit=ms&timeformat=unixtime&timezone=auto";
            }
            if(state.debugWether) {
                Serial.println("OPEN_METEO");
                Serial.println(url);
            }
        }

        else {
            Serial.println("Wrong weather provider");
            return;
        }
        HTTPClient clientWeather;
        clientWeather.begin(url);
        int httpCode = clientWeather.GET();
        if(httpCode == HTTP_CODE_OK) {
            String httpData = clientWeather.getString();
            JsonDocument weather;
            DeserializationError errorWeather = deserializeJson(weather, httpData);
            if(state.debugWether) {
                Serial.println(httpData);
            }
            if(errorWeather) {
                Serial.println("Current weather deserialization error");
                return;
            }

            if(config.weather.provider() == OPENWEATHERMAP) {
                strlcpy(state.weather.descript, weather["weather"][0]["description"] | "", sizeof(state.weather.descript));
                state.weather.temp       = weather["main"]["temp"] | 40400.0;
                state.weather.hum        = weather["main"]["humidity"] | 40400.0;
                state.weather.pres       = weather["main"]["pressure"] | 40400.0;
                state.weather.wind.speed = weather["wind"]["speed"] | -1.0;
                state.weather.wind.dir   = weather["wind"]["deg"] | -1;
                state.weather.icon       = atoi(weather["weather"][0]["icon"] | "0");
                String pod               = weather["weather"][0]["icon"] | "";
                state.weather.isDay      = (pod.substring(2) == "d") ? true : false;
                state.weather.time       = weather["dt"] | 0;
                _lon                     = weather["coord"]["lon"] | 0.0;
                _lat                     = weather["coord"]["lat"] | 0.0;
            }

            if(config.weather.provider() == WEATHERBIT) {
                strlcpy(state.weather.descript, weather["data"][0]["weather"]["description"] | "", sizeof(state.weather.descript));
                state.weather.temp       = weather["data"][0]["temp"] | 40400.0;
                state.weather.hum        = weather["data"][0]["rh"] | 40400.0;
                state.weather.pres       = weather["data"][0]["pres"] | 40400.0;
                state.weather.wind.speed = weather["data"][0]["wind_spd"] | -1.0;
                state.weather.wind.dir   = weather["data"][0]["wind_dir"] | -1;
                const char* pod          = weather["data"][0]["pod"] | "";
                state.weather.isDay      = (String(pod) == String('d')) ? true : false;
                state.weather.icon       = _weatherbitIcon(weather["data"][0]["weather"]["code"].as<int>() | 0);
            }

            if(config.weather.provider() == OPEN_METEO) {
                const char* descr              = _openMeteoCode2Description(weather["current"]["weather_code"] | 0);
                strlcpy(state.weather.descript, descr, sizeof(state.weather.descript));
                state.weather.temp       = weather["current"]["temperature_2m"] | 40400.0;
                state.weather.hum        = weather["current"]["relative_humidity_2m"] | 40400.0;
                state.weather.pres       = weather["current"]["pressure_msl"] | 40400.0;
                state.weather.wind.speed = weather["current"]["wind_speed_10m"] | -1.0;
                state.weather.wind.dir   = weather["current"]["wind_direction_10m"] | -1;
                state.weather.isDay      = weather["current"]["is_day"] == 0 ? false : true;
                state.weather.icon       = _openMeteoIcon(weather["current"]["weather_code"] | 0);
            }

            httpData = "";
            state.weather.time = now();
            Serial.print("Current weather updated successfully at: ");
            Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
        }
        else Serial.println("Current weather update error");
        clientWeather.end();

        #if !defined(BIM32_CYD)
            if(config.display.type(DISPLAY_1) == 1) {
        #endif
                if(config.weather.provider() == WEATHERBIT) _updateWeatherbitDaily();
                if(config.weather.provider() == OPENWEATHERMAP) _updateOpenweathermapHourly();
                if(config.weather.provider() == OPEN_METEO) {
                    _updateOpenMeteoDaily();
                    if(config.display.model(DISPLAY_1) == 0 || config.display.model(DISPLAY_1) == 2) _updateOpenMeteoHourly();
                }
        #if !defined(BIM32_CYD)
            }
        #endif
        state.weather.updated = true;
    }
}