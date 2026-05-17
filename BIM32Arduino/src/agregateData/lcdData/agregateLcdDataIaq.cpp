#include "agregateLcdData.hpp"

#include "../../sensors/sensors.hpp"
#include "../../validation/validate.hpp"

String AgregateLcdData::_iaq() {
    float iaq = sensors.get_bme680_iaq();
    if(validate.iaq(iaq)) return ("IAQ " + String((int)round(iaq)));
    else return "--";
}