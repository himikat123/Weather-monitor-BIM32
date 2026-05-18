#pragma once
#include <Arduino.h>

class MAX7219_Driver {
    public:
        void init(int8_t clk, int8_t dat, int8_t load, uint8_t chip);
        void power(bool state);
        void setIntensity(uint8_t intensity);
        void print(byte* img);
        void clearDisplay();

    private:
        int8_t _clk = -1;
        int8_t _dat = -1;
        int8_t _load = -1;
        uint8_t _chip = 0;

        void _spiTransfer(int addr, volatile byte opcode, volatile byte data);
};