#include <Arduino.h>
#include <TimeLib.h>

#include "./timeNTP.hpp"
#include "../state/state.hpp"
#include "../config/config.hpp"
#include "../sensors/sensors.hpp"
#include "../displays/nextion/nextion.hpp"

/**
 * Time synchronization with NTP server
 */
void TimeNTP::syncNTP(void) {
    if(state.net_connected) {
        configTime(config.clock.utc() * 3600, 0, config.clock.ntp());
        vTaskDelay(pdMS_TO_TICKS(2000));

        struct tm tmstruct;
        if(getLocalTime(&tmstruct)) {
            setTime(tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec, tmstruct.tm_mday, tmstruct.tm_mon + 1, tmstruct.tm_year + 1900);
            if(config.clock.dlst() && isSummertime()) adjustTime(3600);

            state.clockSynchronize = true;
            state.clockSynchronized = true;

            nextion.setDisplayRTC();
            sensors.set_ds3231_timeDate();

            Serial.printf("successful: %02d:%02d:%02d %02d.%02d.%d\r\n", hour(), minute(), second(), day(), month(), year());
        }
        else {
            Serial.println("failed");
            state.clockSynchronized = false;
        }
    }
    else state.clockSynchronized = false;
}

/**
 * Check the time and date for daylight saving time
 */
boolean TimeNTP::isSummertime() {
    int y = year(), m = month(), d = day(), h = hour();

    if(m < 3 || m > 10) return false;
    if(m > 3 && m < 10) return true;
    
    int lastMarchSunday = 31 - ((5 * y / 4 + 4) % 7);
    int lastOctoberSunday = 31 - ((5 * y / 4 + 1) % 7);
    
    if(m == 3) {
        if(d > lastMarchSunday || (d == lastMarchSunday && h >= 2)) return true;
        return false;
    }

    if(m == 10) {
        if(d < lastOctoberSunday || (d == lastOctoberSunday && h < 3)) return true;
        return false;
    }

    return false;
}