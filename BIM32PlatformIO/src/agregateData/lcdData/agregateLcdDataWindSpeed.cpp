#include "agregateLcdData.hpp"

#include "../../config/config.hpp"
#include "../../weather/weather.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"
#include "../../wsensor/wirelessSensor.hpp"

float AgregateLcdData::windSpeed() {
    float windSpeed = UNDEFINED_FLOAT;
    switch(config.display.source.wind.speed.sens()) {
        case WEATHER_WIND_SPEED: { // wind speed from weather forecast
            windSpeed = weather.get_currentWindSpeed();
        }; break;
        case WSENSOR_WIND_SPEED: { // wind speed from wireless sensor
            if(wsensor.dataRelevance(config.display.source.wind.speed.wsensNum()))
                windSpeed = wsensor.get_windSpeed(config.display.source.wind.speed.wsensNum());
        }; break;
        case THING_WIND_SPEED: { // wind speed from thingspeak
            if(thingspeak.dataRelevance()) 
                windSpeed = thingspeak.get_field(config.display.source.wind.speed.thing());
        }; break;
    }
    return windSpeed;
}