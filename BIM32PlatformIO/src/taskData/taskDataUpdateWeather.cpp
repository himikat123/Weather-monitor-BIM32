#include <Arduino.h>
#include <TimeLib.h> 

#include "./taskData.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../network/network.hpp"
#include "../weather/weather.hpp"
#include "../sensors/sensors.hpp"

void TaskData::_updateWeather() { 
    uint32_t weatherUpd = config.weather.provider() == 1 ? 3600 : 1200;
    if(state.debugWether || (now() - weather.get_currentUpdated() > weatherUpd)) {
        Serial.println(SEPARATOR);
        Serial.println("Current weather update... ");
        if(network.isConnected()) weather.update();
        else Serial.println("No internet connection");
        sensors.get_ds3231_timeDate();
        state.debugWether = false;
    }
}