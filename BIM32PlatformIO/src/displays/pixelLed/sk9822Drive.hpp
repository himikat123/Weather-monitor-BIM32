#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "./basePixelLedDisplay.hpp"
#include "../../globals.hpp"
#include "../../pinout.hpp"

class Sk9822DriveSPI2 : public BasePixelLed {
    private:
        NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaSpiMethod>* bus = nullptr;

    public:
        ~Sk9822DriveSPI2() { if (bus) delete bus; }
        
        void begin(uint8_t dispNum, uint16_t count) override {
            bus = new NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaSpiMethod>(count);
            bus->Begin(SK9822_1_CLK_PIN, -1, SK9822_1_DAT_PIN, -1);
        }

        void setPixelColor(uint16_t index, RgbColor color) override {
            if(bus) bus->SetPixelColor(index, color); 
        }

        void clearTo(RgbColor color) override {
            if(bus) bus->ClearTo(color);
        }

        void show() override {
            if(bus) bus->Show();
        }
};

class Sk9822DriveSPI3 : public BasePixelLed {
    private:
        NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaSpi3Method>* bus = nullptr;

    public:
        ~Sk9822DriveSPI3() { if (bus) delete bus; }
        
        void begin(uint8_t dispNum, uint16_t count) override {
            bus = new NeoPixelBus<DotStarBgrFeature, DotStarEsp32DmaSpi3Method>(count);
            bus->Begin(SK9822_2_CLK_PIN, -1, SK9822_2_DAT_PIN, -1);
        }

        void setPixelColor(uint16_t index, RgbColor color) override {
            if(bus) bus->SetPixelColor(index, color);
        }

        void clearTo(RgbColor color) override {
            if(bus) bus->ClearTo(color);
        }

        void show() override {
            if(bus) bus->Show(); 
        }
};