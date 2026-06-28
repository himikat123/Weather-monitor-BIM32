#include <Arduino.h>
#include <TimeLib.h>
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
            t = std::clamp(t, -999, 9999);
            sprintf(buf, "%04d", t);
            for(uint8_t k=0; k<4; k++) dat[k] = buf[k];

            // humidity
            int h = thingspeak.get_historyField(4, i);
            h = std::clamp(h, 0, 999);
            sprintf(buf, "%03d", h);
            for(uint8_t k=0; k<3; k++) dat[4 + k] = buf[k];

            // day
            int d = day(thingspeak.get_historyUpdated(i));
            d = std::clamp(d, 1, 31);
            sprintf(buf, "%02d", d);
            for(uint8_t k=0; k<2; k++) dat[7 + k] = buf[k];

            // month
            int m = month(thingspeak.get_historyUpdated(i)) - 1;
            m = std::clamp(m, 0, 11);
            sprintf(buf, "%02d", m);
            for(uint8_t k=0; k<2; k++) dat[9 + k] = buf[k];

            // hour
            int hr = config.clock.format() > 1 
                ? hour(thingspeak.get_historyUpdated(i)) 
                : hourFormat12(thingspeak.get_historyUpdated(i));
            hr = std::clamp(hr, 0, 23);
            sprintf(buf, "%02d", hr);
            for(uint8_t k=0; k<2; k++) dat[11 + k] = buf[k];

            // minute
            int min = minute(thingspeak.get_historyUpdated(i));
            min = std::clamp(min, 0, 59);
            sprintf(buf, "%02d", min);
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