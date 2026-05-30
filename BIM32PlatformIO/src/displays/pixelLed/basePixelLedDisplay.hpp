#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

class BasePixelLed {
    public:
        virtual ~BasePixelLed() {}
        virtual void begin(uint8_t dispNum,  uint16_t count) = 0;
        virtual void setPixelColor(uint16_t index, RgbColor color) = 0;
        virtual void clearTo(RgbColor color) = 0;
        virtual void show() = 0;
};