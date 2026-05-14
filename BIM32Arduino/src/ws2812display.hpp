#pragma once

#include <LiteLED.h> // v1.2.0 https://github.com/Xylopyrographer/LiteLED/tree/main

LiteLED strip_1(LED_STRIP_WS2812, false, RMT_CHANNEL_2);
LiteLED strip_2(LED_STRIP_WS2812, false, RMT_CHANNEL_3);

#define DISP4 0
#define DISP6 1
#define DOT 100

class WS2812b : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, uint8_t pin);
        void refresh();

    private:
        LiteLED* _strip = nullptr;
        uint8_t _pixelCount = 1;                                // number of pixels in display
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