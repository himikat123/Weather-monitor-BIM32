#include "agregateLcdData.hpp"

#include "../../languages/languages.hpp"
#include "../../config/config.hpp"
#include "../../validation/validate.hpp"
#include "../../wsensor/wirelessSensor.hpp"

String AgregateLcdData::_voltageWsensor() {
    String value = "--";
    if(wsensor.dataRelevance(config.display.source.volt.wsensNum())) {
        switch(config.display.source.volt.volt()) {
            case WSENSOR_BAT_VOLT: { /* battery voltage */
                float volt = wsensor.get_batteryVoltage(config.display.source.volt.wsensNum());
                if(validate.batVolt(volt)) value = String(round(volt * 100) / 100) + lang.v();
            }; break;
            case WSENSOR_BAT_PERCENT: { /* battery percentage */
                float prc = wsensor.get_batteryPercentage(config.display.source.volt.wsensNum());
                if(validate.batPercent(prc)) value = String(prc, 0) + "%";
            }; break;
            case WSENSOR_PZEM_VOLT: { /* PZEM-004t voltage */
                float hiVolt = wsensor.get_voltage(config.display.source.volt.wsensNum());
                if(validate.volt(hiVolt)) value = String(round(hiVolt * 100) / 100) + lang.v();
            }; break;
            case WSENSOR_S8_CO2: { /* SenseAir S8 CO2 level */
                float co2 = wsensor.get_co2(config.display.source.volt.wsensNum());
                if(validate.co2(co2)) value = String((int)round(co2)) + "ppm";
            }; break;
            default: ; break;
        }
    }
    return value;
}