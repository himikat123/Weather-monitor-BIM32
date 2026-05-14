#pragma once

#define DISP4    0
#define DISP6    1
#define DOT    100
#define SYMBOLS  0
#define WITH_DOT 1

class TM1637 : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t clk, int8_t dat);
        void refresh();

    private:
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