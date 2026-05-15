#pragma once
#include <Arduino.h>

#include "./tm1637driver.hpp"
#include "../segmentDisplay.hpp"

class TM1637 : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t clk, int8_t dat);
        void refresh();

    protected:
        TM1637_Driver tm1637;

    private:
        static constexpr int DISP4    = 0;
        static constexpr int DISP6    = 1;
        static constexpr int WDOT     = 100;
        static constexpr int SYMBOLS  = 0;
        static constexpr int WITH_DOT = 1;

        int8_t _clk = -1;
        int8_t _dat = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        void _print();
        void _sendToDisplay();
        void _clearDisplay();
        void _writeByte(byte data);
        void _start();
        void _stop(void);
};