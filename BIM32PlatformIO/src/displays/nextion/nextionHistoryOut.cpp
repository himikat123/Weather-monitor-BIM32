#include <Arduino.h>
#include <TimeLib.h>
#include "./nextion.hpp"

#include "../../config/config.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"

/**
 * Sending data to display outdoor weather history
 */
void Nextion::_historyOut() {
    if(_prevHistoryOutChecksum != _historyOutChecksum or _forced) {
        char dat[18] = "";
        char buf[20] = "";
        Serial1.print("HistoryOut.data0.txt=\"");
        for(uint8_t i=0; i<24; i++) {
            // temperature
            float temp = thingspeak.get_historyField(0, i);
            int t = round(temp * 10);
            sprintf(buf, "%04d", t);
            for(uint8_t k=0; k<4; k++) dat[k] = buf[k];
            // humidity
            int h = thingspeak.get_historyField(1, i);
            sprintf(buf, "%03d", h);
            for(uint8_t k=0; k<3; k++) dat[4 + k] = buf[k];
            // pressure
            int p = round(thingspeak.get_historyField(2, i) * 0.75);
            sprintf(buf, "%03d", p);
            for(uint8_t k=0; k<3; k++) dat[7 + k] = buf[k];
            // day
            sprintf(buf, "%02d", day(thingspeak.get_historyUpdated(i)));
            for(uint8_t k=0; k<2; k++) dat[10 + k] = buf[k];
            // month
            sprintf(buf, "%02d", month(thingspeak.get_historyUpdated(i)) - 1);
            for(uint8_t k=0; k<2; k++) dat[12 + k] = buf[k];
            // hour
            unsigned int hr = config.clock.format() > 1 
                ? hour(thingspeak.get_historyUpdated(i)) 
                : hourFormat12(thingspeak.get_historyUpdated(i));
            sprintf(buf, "%02d", hr);
            for(uint8_t k=0; k<2; k++) dat[14 + k] = buf[k];
            // minute
            sprintf(buf, "%02d", minute(thingspeak.get_historyUpdated(i)));
            for(uint8_t k=0; k<2; k++) dat[16 + k] = buf[k];
            // send current slot
            for(uint8_t k=0; k<18; k++) Serial1.print(dat[k]);
        }
        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        _prevHistoryOutChecksum = _historyOutChecksum;
    }
}