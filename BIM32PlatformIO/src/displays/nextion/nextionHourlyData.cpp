#include <Arduino.h>
#include "./nextion.hpp"

#include "../../globals.hpp"
#include "../../config/config.hpp"
#include "../../weather/weather.hpp"
#include "../../agregateData/lcdData/agregateLcdData.hpp"

/**
 * Sending data to display hourly forecast
 */
void Nextion::_hourlyData() {
    if((_prevHourlyChecksum != _hourlyChecksum) or _forced) {
        char dat[23] = "";
        char buf[20] = "";
        Serial1.print("Hourly.data0.txt=\"");

        for(uint8_t i=0; i<40; i++) {
            // temp
            float temp = weather.get_hourlyTemp(i);
            int t = round(temp * 10);
            sprintf(buf, "%04d", t);
            for(uint8_t k=0; k<4; k++) dat[k] = buf[k];
            // pres
            float pres = weather.get_hourlyPres(i);
            unsigned int p = round(pres);
            sprintf(buf, "%04d", p);
            for(uint8_t k=0; k<4; k++) dat[4 + k] = buf[k];
            // icon
            sprintf(buf, "%02d", weather.get_hourlyIcon(i));
            for(uint8_t k=0; k<2; k++) dat[8 + k] = buf[k];
            // weekday
            sprintf(buf, "%d", weekday(weather.get_hourlyDate(i)) - 1);
            dat[10] = buf[0];
            // day
            sprintf(buf, "%02d", day(weather.get_hourlyDate(i)));
            for(uint8_t k=0; k<2; k++) dat[11 + k] = buf[k];
            // month
            sprintf(buf, "%02d", month(weather.get_hourlyDate(i)) - 1);
            for(uint8_t k=0; k<2; k++) dat[13 + k] = buf[k];
            // hour
            unsigned int hr = config.clock.format() > 1 
                ? hour(weather.get_hourlyDate(i)) 
                : hourFormat12(weather.get_hourlyDate(i));
            sprintf(buf, "%02d", hr);
            for(uint8_t k=0; k<2; k++) dat[15 + k] = buf[k];
            // wind speed
            unsigned int wind = round(weather.get_hourlyWindSpeed(i));
            sprintf(buf, "%02d", wind);
            for(uint8_t k=0; k<2; k++) dat[17 + k] = buf[k];
            // wind direction
            unsigned int deg = agregateLcdData.windDirection(weather.get_hourlyWindDir(i));
            if(deg > 7) deg = 0;
            sprintf(buf, "%d", deg);
            dat[19] = buf[0];
            // precipitation
            int pr = 0;
            if(config.weather.provider() == OPEN_METEO) pr = weather.get_hourlyPrec(i);
            else pr = round(weather.get_hourlyPrec(i) * 100);
            sprintf(buf, "%03d", pr);
            for(uint8_t k=0; k<3; k++) dat[20 + k] = buf[k];

            // send all to display
            for(uint8_t k=0; k<23; k++) Serial1.print(dat[k]);
        }
        Serial1.print("\"");
        Serial1.write(0xFF);
        Serial1.write(0xFF);
        Serial1.write(0xFF);

        _daily2hourly();
        _prevHourlyChecksum = _hourlyChecksum;
    }
}