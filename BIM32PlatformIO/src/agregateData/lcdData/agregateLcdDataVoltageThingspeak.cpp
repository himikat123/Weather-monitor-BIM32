#include "agregateLcdData.hpp"

#include "../../config/config.hpp"
#include "../../validation/validate.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"

String AgregateLcdData::_voltageThingspeak() {
    String value = "--";
    if(thingspeak.dataRelevance()) {
        switch(config.display.source.volt.thingType()) {
            case THING_BAT_VOLT: { /* battery voltage */
                float volt = thingspeak.get_field(config.display.source.volt.thing());
                if(validate.thingVolt(volt)) value = String(round(volt * 100) / 100) + lang.v();
            }; break;
            case THING_BAT_PERCENT: { /* battery percentage */
                float prc = thingspeak.get_field(config.display.source.volt.thing());
                if(validate.batPercent(prc)) value = String(prc, 0) + "%";
            }; break;
            default: ; break;
        }
    }
    return value;
}