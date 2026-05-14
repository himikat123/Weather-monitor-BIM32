#pragma once

#define DISP4    0
#define DISP6    1
#define DOT    100
#define SYMBOLS  0
#define WITH_DOT 1

class PCF8575_S : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t scl, int8_t sda, int8_t pwm, int8_t ws);
        void brightness(uint8_t intensity, bool reduc);
        void displayToggle();
        void displayOn();
        void displayOff();
        void refresh();

    private:
        SoftI2C* _wire = nullptr;
        LiteLED* _strip = nullptr;
        int8_t _pwm = -1;
        int8_t _ws = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _prevPixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        void _print();
        void _sendToDisplay();
        void _clearDisplay();
};