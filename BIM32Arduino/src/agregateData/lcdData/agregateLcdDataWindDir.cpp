#include "agregateLcdData.hpp"

#include "../../config.hpp"
#include "../../weather.hpp"
#include "../../thingspeak.hpp"
#include "../../wirelessSensor.hpp"

int AgregateLcdData::windDir() {
    int windDir = UNDEFINED_FLOAT;
    switch(config.display.source.wind.dir.sens()) {
        case WEATHER_WIND_DIR: { // wind directory from weather forecast
            windDir = weather.get_currentWindDir(); 
        }; break;
        case WSENSOR_WIND_DIR: { // wind directory from wireless sensor
            if(wsensor.dataRelevance(config.display.source.wind.dir.wsensNum()))
                windDir = wsensor.get_windDir(config.display.source.wind.dir.wsensNum());
        }; break;
        case THING_WIND_DIR: { // wind directory from thingspeak
            if(thingspeak.dataRelevance()) 
                windDir = round(thingspeak.get_field(config.display.source.wind.dir.thing()));
        }; break;
    }
    return windDir;
}