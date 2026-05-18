#include "agregateLcdData.hpp"

#include "../../config/config.hpp"
#include "../../state/state.hpp"

/**
 * Voltage color
 * @return 0: default color, 1-3: air quality level
 */
uint8_t AgregateLcdData::voltageColor() {
    uint8_t type = 0;
    switch(config.display.source.volt.sens()) {
        case WSENSOR_S8_COLOR: { /* from wireless sensor */
            if(config.display.source.volt.volt() == 3) /* SenseAir S8 CO2 level */
                type = state.co2_level;
        }; break; 
        case BME680_IAQ_COLOR: { // iaq from BME680
            type = state.iaq_level;
        }; break;
        default: ; break;
    }
    return type;
}