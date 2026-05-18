#pragma once
#include <Arduino.h>

class TimeNTP {
    public:
        void syncNTP(void);
        boolean isSummertime();
};