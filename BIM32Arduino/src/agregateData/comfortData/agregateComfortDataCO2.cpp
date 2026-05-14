#include "agregateComfortData.hpp"

#include "../../globals.hpp"
#include "../../config.hpp"
#include "../../wirelessSensor.hpp"

float AgregateComfortData::co2() {
    int co2 = UNDEFINED_FLOAT;
    if(config.comfort.co2.source() == WSENSOR_CO2) {
        if(wsensor.dataRelevance(config.comfort.co2.wsensNum()))
            co2 = wsensor.get_co2(config.comfort.co2.wsensNum());
    }
    return co2;
}