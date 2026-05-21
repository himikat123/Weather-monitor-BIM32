#pragma once
#include <Arduino.h>

#include "../baseDisplay.hpp"
#include "../../agregateData/segmentData/agregateSegmentData.hpp"

class SegmentDisplay : public SegmentDisplay {
    public:
        void setDotFreq(unsigned int freq);
        void displayToggle();
        virtual bool isDisplayOn() const override { return false; }
        virtual void displayOff() override {}
        virtual void displayOn() override {}
        void brightness(uint8_t intensity, bool reduc);

    protected:
        uint8_t _slot = 0;
        bool _points[8] = {false, false, false, false, false, false, false, false};
        bool _prevPoints[8] = {false, false, false, false, false, false, false, false};
        unsigned int _dotfreq = 500;    // time points period
        bool _power = true;             // display on/off flag
        uint8_t _brightness = 40;       // display brightness
        int _dispImg[8] = {             // display digits
            SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE, SYMB_SPACE
        };
        char _dispColors[8][8] = {"", "", "", "", "", "", "", ""};
        uint8_t _dispNum = 0;
        bool _animIsRunnung = false;
        uint8_t _dispLength = 4;

        void _slotSwitch();
        void _segAnimations();
        void _setModel(uint8_t model);

    private:
        AgregateSegmentData agregateSegmentData;
        static constexpr int SYMB_DEGREE = 0x0A;
        static constexpr int SYMB_A      = 0x0B;
        static constexpr int SYMB_C      = 0x0C;
        static constexpr int SYMB_P      = 0x0D;
        static constexpr int SYMB_H      = 0x0E;
        static constexpr int SYMB_F      = 0x11;
        static constexpr int SYMB_SPACE  = 0x0F;
        static constexpr int SYMB_MINUS  = 0x10;

        uint8_t _prevSlot = 0;
        uint8_t _animSlot = 0;
        uint32_t _animMillis = 0;
        uint32_t _prevSlotMillis = 0;
        uint16_t _millisShift = 0;
        uint8_t _prevSecond = 60;
        bool _pointsState = false;

        void _segGetData(int* segImg, uint8_t slot, bool dots);
        int _pendulumPattern(uint32_t ml, uint8_t max);
        void _clock(int* segImg, uint8_t slot);
        void _date(int* segImg, uint8_t slot);
        void _temp(float t, int* segImg);
        void _hum(float h, int* segImg);
        void _pres(float p, int* segImg);
        void _iaq(float i, int* segImg);
        void _co2(float c, int* segImg);
        void _apMode(int* segImg);
};