#pragma once
#include <Arduino.h>

class BaseDisplay {
    public:
        virtual ~BaseDisplay() {}

        virtual bool isdisplayOn() const = 0;
        virtual void displayOn(bool doinit = true) = 0;
        virtual void displayOff() = 0;
        virtual void setDotFreq(uint16_t dotFreq) = 0;
        virtual void brightness(unsigned int bright, bool reduc) = 0;
};