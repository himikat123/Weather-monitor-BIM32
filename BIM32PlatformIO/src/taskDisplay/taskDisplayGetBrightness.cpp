#include <Arduino.h>
#include <TimeLib.h>

#include "./taskDisplay.hpp"
#include "../config/config.hpp"
#include "../validation/validate.hpp"
#include "../sensors/sensors.hpp"
#include "../weather/weather.hpp"

/**
 * Display brightness calculation
 */
unsigned int TaskDisplay::_get_brightness() {
    switch(config.display.brightness.method(_dispNum)) {

        /* By sunrise and sunset */
        case 0:
            return weather.get_isDay() 
                ? config.display.brightness.day(_dispNum) 
                : config.display.brightness.night(_dispNum);

        /* By ambeint light sensor */
        case 1: {
            float bright = 0.0;
            if(config.display.lightSensor.type(_dispNum) == 0) {
                bright = validate.volt(sensors.get_analog_voltage()) 
                    ? sensors.get_analog_voltage() * 30 
                    : 20;
            }
            if(config.display.lightSensor.type(_dispNum) == 1) {
                bright = validate.light(sensors.get_max44009_light()) 
                    ? sensors.get_max44009_light() 
                    : 20;
            }
            if(config.display.lightSensor.type(_dispNum) == 2) {
                bright = validate.light(sensors.get_bh1750_light()) 
                    ? sensors.get_bh1750_light() 
                    : 20;
            }
            bright *= (float)config.display.lightSensor.sensitivity(_dispNum) / 20.0;
            if(bright < 1.0) bright = 1.0;
            if(bright > 100.0) bright = 100.0;
            return round(bright);
        };

        /* By user defined time */
        case 2: {
            unsigned int morning = config.display.dayTime(_dispNum, HOUR) * 60 + config.display.dayTime(_dispNum, MINUTE);
            unsigned int evening = config.display.nightTime(_dispNum, HOUR) * 60 + config.display.nightTime(_dispNum, MINUTE);
            unsigned int cur_time = int(hour()) * 60 + minute();
            if(morning < evening) {
                if(morning <= cur_time && cur_time < evening) return config.display.brightness.day(_dispNum);
            }
            else {
                if(cur_time >= morning || cur_time < evening) return config.display.brightness.day(_dispNum);
            }
            return config.display.brightness.night(_dispNum);
        };

        /* Constant brightness */
        case 3: return config.display.brightness.day(_dispNum);

        default: return 50; break;
    }
}