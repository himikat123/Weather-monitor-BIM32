#include "agregateLcdData.hpp"

#include "../../validate.hpp"
#include "../../sensors.hpp"

String AgregateLcdData::_dewPoint(float temp, float hum) {
    float dp = sensors.dewPoint(temp, hum);
    if(validate.dewPoint(dp, temp)) {
        float value = dp;
        value = round(value * 10) / 10.0;
        String buf = String(value, 1) + "°C";
        return buf;
    }
    else return "--";
}