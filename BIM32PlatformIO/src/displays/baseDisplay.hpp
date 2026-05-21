#pragma once

class BaseDisplay {
    public:
        virtual ~BaseDisplay() {}

        virtual bool isDisplayOn() const = 0;
        virtual void displayOff() = 0;
        virtual void displayOn() = 0;
        
};