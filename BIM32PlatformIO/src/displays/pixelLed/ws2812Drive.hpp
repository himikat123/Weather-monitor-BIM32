#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "./basePixelLedDisplay.hpp"
#include "../../globals.hpp"
#include "../../pinout.hpp"

class Ws2812DriveI2S : public BasePixelLed {
    private:
        NeoPixelBus<NeoGrbFeature, NeoEsp32I2s0Ws2812xMethod>* bus = nullptr;
        //using StripType = NeoPixelBus<NeoRgbFeature, NeoEsp32Rmt0Apa106Method>;

    public:
        ~Ws2812DriveI2S() { if (bus) delete bus; }

        void begin(uint8_t dispNum, uint16_t count) override {
            bus = new NeoPixelBus<NeoGrbFeature, NeoEsp32I2s0Ws2812xMethod>(count, dispNum == DISPLAY_1 ? WS2812_1_DAT_PIN : WS2812_2_DAT_PIN);
            bus->Begin();
        }

        void setPixelColor(uint16_t index, RgbColor color) override { bus->SetPixelColor(index, color); }
        void clearTo(RgbColor color) override { bus->ClearTo(color); }
        void show() override { bus->Show(); }
};

class Ws2812DriveRMT : public BasePixelLed {
    private:
        NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>* bus = nullptr;

    public:
        ~Ws2812DriveRMT() { if (bus) delete bus; }

        void begin(uint8_t dispNum, uint16_t count) override {
            bus = new NeoPixelBus<NeoGrbFeature, NeoEsp32Rmt0Ws2812xMethod>(count, dispNum == DISPLAY_1 ? WS2812_1_DAT_PIN : WS2812_2_DAT_PIN);
            bus->Begin();
        }

        void setPixelColor(uint16_t index, RgbColor color) override { bus->SetPixelColor(index, color); }
        void clearTo(RgbColor color) override { bus->ClearTo(color); }
        void show() override { bus->Show(); }
};