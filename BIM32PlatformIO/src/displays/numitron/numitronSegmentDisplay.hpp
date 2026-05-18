#pragma once
#include <Arduino.h>
#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main

#include "./softi2c.hpp"
#include "../segmentDisplay.hpp"

class Numitron : public SegmentDisplay {
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