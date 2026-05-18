#include "agregateLcdData.hpp"

#include "../../config/config.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"
#include "../../wsensor/wirelessSensor.hpp"

int AgregateLcdData::batteryLevel() {
    int level = -1;
    if(config.display.source.bat.sens() == WSENSOR_BAT_SYMB) { // battery symbol from wireless sensor
        if(wsensor.dataRelevance(config.display.source.bat.wsensNum())) {
            level = wsensor.get_batteryLevel(config.display.source.bat.wsensNum());
        }
    }
    else if(config.display.source.bat.sens() == THING_BAT_SYMB) { // battery symbol from thingspeak
        if(thingspeak.dataRelevance()) {
            level = thingspeak.get_field(config.display.source.bat.thing());
            if(level == 5) level = 4;
        }
    }
    return level;
}