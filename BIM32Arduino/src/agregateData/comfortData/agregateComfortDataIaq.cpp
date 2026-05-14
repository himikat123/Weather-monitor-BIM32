#include "agregateComfortData.hpp"

#include "../../globals.hpp"
#include "../../config.hpp"
#include "../../sensors.hpp"

float AgregateComfortData::iaq() {
    float iaq = UNDEFINED_FLOAT;
    if(config.comfort.iaq.source() == BME680_IAQ) {
        iaq = sensors.get_bme680_iaq();
    }
    return iaq;
}