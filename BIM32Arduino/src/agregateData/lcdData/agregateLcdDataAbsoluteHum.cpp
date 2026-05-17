#include "agregateLcdData.hpp"

#include "../../sensors/sensors.hpp"
#include "../../validation/validate.hpp"
#include "../../languages/languages.hpp"

String AgregateLcdData::_absoluteHum(float temp, float hum) {
    float ah = sensors.absoluteHum(temp, hum);
    if(validate.absoluteHum(ah)) {
        ah = round(ah * 10) / 10.0;
        String buf = String(ah, 1);
        buf += lang.gpm();
        return buf;
    }
    else return "--";
}