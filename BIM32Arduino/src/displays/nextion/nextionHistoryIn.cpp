#include <Arduino.h>
#include "./nextion.hpp"

#include "../../config/config.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"

/**
 * Sending data to display indoor weather history
 */
void Nextion::_historyIn() {
    if(_prevHistoryInChecksum != _historyInChecksum or _forced) {
        char dat[15] = "";
        char buf[20] = "";
        Serial1.print("HistoryIn.data0.txt=\"");
        for(uint8_t i=0; i<24; i++) {
            // temperature
            float temp = thingspeak.get_historyField(3, i);
            int t = round(temp * 10);
            sprintf(buf, "%04d", t);
            for(uint8_t k=0; k<4; k++) dat[k] = buf[k];
            // humidity
            int h = thingspeak.get_historyField(4, i);
            sprintf(buf, "%03d", h);
            for(uint8_t k=0; k<3; k++) dat[4 + k] = buf[k];
            // day
            sprintf(buf, "%02d", day(thingspeak.get_historyUpdated(i)));
            for(uint8_t k=0; k<2; k++) dat[7 + k] = buf[k];
            // month
            sprintf(buf, "%02d", month(thingspeak.get_historyUpdated(i)) - 1);
            for(uint8_t k=0; k<2; k++) dat[9 + k] = buf[k];
            // hour
            unsigned int hr = config.clock.format() > 1 
                ? hour(thingspeak.get_historyUpdated(i)) 
                : hourFormat12(thingspeak.get_historyUpdated(i));
            sprintf(buf, "%02d", hr);
            for(uint8_t k=0; k<2; k++) dat[11 + k] = buf[k];
            // minute
            sprintf(buf, "%02d", minute(thingspeak.get_historyUpdated(i)));
            for(uint8_t k=0; k<2; k++) dat[13 + k] = buf[k];
            // send current slot
            for(uint8_t k=0; k<15; k++) Serial1.print(dat[k]);
        }
        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        _prevHistoryInChecksum = _historyInChecksum;
    }
}