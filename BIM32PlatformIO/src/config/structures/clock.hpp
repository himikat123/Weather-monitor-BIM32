#pragma once
#include <Arduino.h>

class Config;

struct ClockStruct {
    private:
        unsigned int _format = 0; // Clock format: 0 = 12 hour wo leading zero, 1 = 12 hour with leading zero, 2 = 24 hour wo leading zero, 3 = 24 hour with leading zero
        char _ntp[65] = "time.nist.gov"; // NTP server address
        int _utc = 0; // Timezone -12...13
        unsigned int _dlst = 0; // Auto daylight saving time
        unsigned int _ntpPeriod = 15; // NTP update period (minutes) 0...90000
        friend class Config;

    public:
        unsigned int format() const { if(_format > 3) return 0; return _format; }
        const char* ntp() const { if(String(_ntp) == "") return (char*) "time.nist.gov"; return _ntp; }
        const int utc() const { if(_utc < -12 or _utc > 13) return 0; return _utc; }
        const unsigned int ntpPeriod() const { if(_ntpPeriod > 90000) return 15; return _ntpPeriod; }
        const bool dlst() const { return _dlst ? true : false; }
};