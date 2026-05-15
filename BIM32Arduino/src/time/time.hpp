#pragma once
#include <Arduino.h>

class Time {
    public:
        void syncNTP(void);
        boolean isSummertime();
};

extern Time time;