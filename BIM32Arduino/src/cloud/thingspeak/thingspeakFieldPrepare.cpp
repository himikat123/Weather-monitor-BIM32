#include <Arduino.h>
#include "./thingspeak.hpp"

#include "../../time/time.hpp"
#include "../../agregateData/sendData/agregateSendData.hpp"
#include "../../config.hpp"

/**
 * Prepare data for the desired field according to the settings
 * @param field number
 * @return a string with field name and data
 */
String Thingspeak::_fieldPrepare(unsigned int fieldNum) {
    float data = agregateSendData.sendingData(
        config.cloud.thingspeakSend.fields(fieldNum),
        config.cloud.thingspeakSend.types(fieldNum),
        config.cloud.thingspeakSend.wsensors(fieldNum),
        config.cloud.thingspeakSend.wtypes(fieldNum)
    );

    if(data != -40400.0) {
        String field = "&field" + String(fieldNum + 1) + "=" + String(data);
        return field;
    }
    return "";
}