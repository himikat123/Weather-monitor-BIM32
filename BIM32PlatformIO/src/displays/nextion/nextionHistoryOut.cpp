#include <algorithm>
#include <Arduino.h>
#include <TimeLib.h>
#include "./nextion.hpp"

#include "../../config/config.hpp"
#include "../../sensors/sensors.hpp"
#include "../../cloud/thingspeak/thingspeak.hpp"

/**
 * Sending data to display outdoor weather history
 */
void Nextion::_historyOut() {
    if(_prevHistoryOutChecksum != _historyOutChecksum or _forced) {
        char dat[19] = "";
        char buf[20] = "";
        Serial1.print("HistoryOut.data0.txt=\"");

        for(uint8_t i=0; i<24; i++) {

            // temperature
            float temp = thingspeak.get_historyField(0, i);
            int t = round(temp * 10);
            t = std::clamp(t, -999, 9999);
            sprintf(buf, "%04d", t);
            for(uint8_t k=0; k<4; k++) dat[k] = buf[k];

            // humidity
            int h = thingspeak.get_historyField(1, i);
            h = std::clamp(h, 0, 999);
            sprintf(buf, "%03d", h);
            for(uint8_t k=0; k<3; k++) dat[4 + k] = buf[k];

            // pressure
            float pr = thingspeak.get_historyField(2, i);
            int p = round(config.units_pres() == HPA ? pr : sensors.mmHg(pr));
            p = std::clamp(p, 0, 9999);
            sprintf(buf, "%04d", p);
            for(uint8_t k=0; k<4; k++) dat[7 + k] = buf[k];

            // day
            int d = day(thingspeak.get_historyUpdated(i));
            d = std::clamp(d, 1, 31);
            sprintf(buf, "%02d", d);
            for(uint8_t k=0; k<2; k++) dat[11 + k] = buf[k];

            // month
            int m = month(thingspeak.get_historyUpdated(i)) - 1;
            m = std::clamp(m, 0, 11);
            sprintf(buf, "%02d", m);
            for(uint8_t k=0; k<2; k++) dat[13 + k] = buf[k];

            // hour
            int hr = config.clock.format() > 1 
                ? hour(thingspeak.get_historyUpdated(i)) 
                : hourFormat12(thingspeak.get_historyUpdated(i));
            hr = std::clamp(hr, 0, 23);
            sprintf(buf, "%02d", hr);
            for(uint8_t k=0; k<2; k++) dat[15 + k] = buf[k];

            // minute
            int min = minute(thingspeak.get_historyUpdated(i));
            min = std::clamp(min, 0, 59);
            sprintf(buf, "%02d", min);
            for(uint8_t k=0; k<2; k++) dat[17 + k] = buf[k];

            // send current slot
            for(uint8_t k=0; k<19; k++) Serial1.print(dat[k]);
        }

        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        _prevHistoryOutChecksum = _historyOutChecksum;
    }
}