#pragma once

#include <Arduino.h>
#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main

#include "../segmentDisplay/segmentDisplay.hpp"

class WS2812b : public SegmentDisplay {
    protected:
        LiteLED strip_1;
        LiteLED strip_2;

    public:
        WS2812b() : 
            strip_1(LED_STRIP_WS2812, false, RMT_CHANNEL_2), 
            strip_2(LED_STRIP_WS2812, false, RMT_CHANNEL_3) 
        {}
        void init(uint8_t dispNum, uint8_t pin);
        void refresh();
    
    private:
        LiteLED* _strip = nullptr;
        uint8_t _pixelCount = 1;
        byte _pixels[6] = {0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _reds[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _redsPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _greens[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _greensPrev[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _blues[6] = {0, 0, 0, 0, 0, 0};
        uint8_t _bluesPrev[6] = {0, 0, 0, 0, 0, 0};

        void _print();
        void _sendToDisplay();
        uint8_t _sendTwoDigits(rgb_t black, uint8_t digShift, uint8_t pixelShift);
};