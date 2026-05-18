#include <Arduino.h>
#include <OpenWeather.h> // v0.3.0 https://github.com/Bodmer/OpenWeather/tree/main?tab=readme-ov-file
#include <JSON_Decoder.h> // https://github.com/Bodmer/JSON_Decoder

#include "./weather.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"

/**
 * Update 3 hourly forecast from openweathermap.org
 */
void Weather::_updateOpenweathermapHourly(void) {
    Serial.println(SEPARATOR);
    Serial.println("Hourly forecast update... ");

    OW_Weather ow;
    OW_forecast  *forecast;
    forecast = new OW_forecast;
    bool parsed = ow.getForecast(
        forecast, 
        String(config.weather.appid(OPENWEATHERMAP)), 
        String(_lat), 
        String(_lon), 
        "metric", "en", false
    );

    if(parsed) {
        state.weather.hourly.time = now();

        for(unsigned int n=0; n<DAYS*8; n++) {
            state.weather.hourly.date[n] = forecast->dt[n];
            state.weather.hourly.icon[n] = forecast->icon[n].toInt();
            state.weather.hourly.temp[n] = forecast->temp[n];
            state.weather.hourly.pres[n] = forecast->pressure[n];
            state.weather.hourly.windSpeed[n] = round(forecast->wind_speed[n]);
            state.weather.hourly.windDir[n] = forecast->wind_deg[n];
            state.weather.hourly.prec[n] = forecast->prec[n];
        }

        Serial.print("Hourly forecast updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());

        _calculateDaily();
    }
    else Serial.println("Hourly forecast update error");
    delete forecast;
}