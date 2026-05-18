#pragma once
#include <Arduino.h>

class TM1637_Driver {
    public:
        void init(int8_t clk, int8_t dat);
        void writeByte(byte data);
        void start();
        void stop(void);

    private:
        int8_t _clk = -1;
        int8_t _dat = -1;
};