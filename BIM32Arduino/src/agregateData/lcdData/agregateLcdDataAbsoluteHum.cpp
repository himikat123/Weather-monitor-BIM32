#include "agregateLcdData.hpp"

#include "../../sensors.hpp"
#include "../../validate.hpp"
#include "../../languages.hpp"

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