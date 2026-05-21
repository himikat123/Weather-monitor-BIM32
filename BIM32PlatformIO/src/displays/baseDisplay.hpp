#pragma once

class BaseDisplay {
    public:
        virtual ~BaseDisplay() {}

        virtual bool isdisplayOn() const = 0;
        virtual void displayOn(bool doinit = true) = 0;
        virtual void displayOff() = 0;
        
};