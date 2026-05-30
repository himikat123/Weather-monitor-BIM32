#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "./basePixelLedDisplay.hpp"
#include "../../globals.hpp"
#include "../../pinout.hpp"

class Sk9822Drive : public BasePixelLed {
    private:
        NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaSpiMethod>* bus1 = nullptr;
        NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaSpi3Method>* bus2 = nullptr;
        uint8_t _disp = 0;

    public:
        ~Sk9822Drive() { if (bus1) delete bus1; if (bus2) delete bus2; }
        
        void begin(uint8_t dispNum, uint16_t count) override {
            _disp = dispNum;
            if(dispNum == DISPLAY_1) {
                bus1 = new NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaSpiMethod>(count);
                bus1->Begin(SK9822_1_DAT_PIN, -1, SK9822_1_CLK_PIN, -1);
            }
            else {
                bus2 = new NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaSpi3Method>(count);
                bus2->Begin(SK9822_2_DAT_PIN, -1, SK9822_2_CLK_PIN, -1);
            }
        }

        void setPixelColor(uint16_t index, RgbColor color) override {
            if(_disp == DISPLAY_1 && bus1) bus1->SetPixelColor(index, color);
            if(_disp != DISPLAY_1 && bus2) bus2->SetPixelColor(index, color);
        }

        void clearTo(RgbColor color) override {
            if(_disp == DISPLAY_1 && bus1) bus1->ClearTo(color);
            if(_disp != DISPLAY_1 && bus2) bus2->ClearTo(color);
        }

        void show() override {
            if(_disp == DISPLAY_1 && bus1) bus1->Show();
            if(_disp != DISPLAY_1 && bus2) bus2->Show();
        }
};