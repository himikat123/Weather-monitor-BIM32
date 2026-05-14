#pragma once

#include "./max7219driver.hpp"

#define DISP4 0
#define DISP6 1
#define DISP8 2

class MAX7219_S : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t clk, int8_t dat, int8_t load);
        void refresh();
        void brightness(uint8_t intensity, bool reduc);
        void displayToggle();
        void displayOn();
        void displayOff();

    private:
        MAX7219_Driver max7219;
        int8_t _clk = -1;
        int8_t _dat = -1;
        int8_t _load = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        void _print();
        void _sendToDisplay();
        void _clearDisplay();
};