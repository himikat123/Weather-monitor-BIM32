#pragma once

#include <Arduino.h>
#include <NeoPixelBus.h>

#include "./basePixelLedDisplay.hpp"
#include "../segmentDisplay/segmentDisplay.hpp"

class PixelLed : public SegmentDisplay {
    public:
        using ColorType = RgbColor;

        PixelLed() : _driver(nullptr) {}
        virtual ~PixelLed() { if (_driver) delete _driver; }
        
        void init(uint8_t dispNum, uint8_t pin);
        virtual void refresh() override;
        virtual bool isdisplayOn() const override { return false; }
        virtual void displayOff() override {}
        virtual void displayOn(bool doinit) override {}
    
    private:
        BasePixelLed* _driver;
        uint8_t _currentBrightness = 1;
        uint8_t _pixelCount = 1;
        byte _pixels[6] = { 0 }; 
        byte _pixelsPrev[6] = { 0 };
        uint8_t _reds[6] = { 0 };
        uint8_t _redsPrev[6] = { 0 };
        uint8_t _greens[6] = { 0 };
        uint8_t _greensPrev[6] = { 0 };
        uint8_t _blues[6] = { 0 };
        uint8_t _bluesPrev[6] = { 0 };

        void _print();
        void _sendToDisplay();
        uint8_t _sendTwoDigits(ColorType black, uint8_t digShift, uint8_t pixelShift); 
};