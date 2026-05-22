#pragma once

#include "./max7219driver.hpp"
#include "../segmentDisplay/segmentDisplay.hpp"

class MAX7219_S : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t clk, int8_t dat, int8_t load);
        virtual void refresh() override;
        virtual void displayToggle() override;
        virtual void displayOff() override;
        virtual void displayOn(bool doinit) override;
        virtual bool isdisplayOn() const override;
        virtual void setDotFreq(uint16_t dotFreq) override;
        virtual void brightness(unsigned int bright, bool reduc) override;

    protected:
        MAX7219_Driver max7219;

    private:
        int8_t _clk = -1;
        int8_t _dat = -1;
        int8_t _load = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        void _print();
        void _sendToDisplay();
        void _clearDisplay();
};