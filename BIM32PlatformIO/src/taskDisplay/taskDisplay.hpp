#pragma once
#include <Arduino.h>

#pragma once

#include "../displays/baseDisplay.hpp"
#include "./displays/nextion/nextion.hpp"
#include "./displays/ili9341/ili9341.hpp"
#include "./displays/ws2812/ws2812display.hpp"
#include "./displays/max7219/max7219segmentDisplay.hpp"
#include "./displays/tm1637/tm1637display.hpp"
#include "./displays/numitron/numitronSegmentDisplay.hpp"

class TaskDisplay {
    protected:
        

    public:
        TaskDisplay();
        ~TaskDisplay();

        bool start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId, int dispNum);

    private:
        TaskHandle_t _taskHandle = nullptr;
        static void _taskWrapper(void* pvParameters);
        void _run();

        BaseDisplay* _display = nullptr;
        Nextion _nextion;
        ILI9341 _ili9341;
        WS2812b ws2812b_1;
        WS2812b ws2812b_2;
        MAX7219_S max7219s_1;
        MAX7219_S max7219s_2;
        TM1637 tm1637_1;
        TM1637 tm1637_2;
        Numitron numitron_1;
        Numitron numitron_2;

        int _dispNum = 0;

        bool _isNightOffTime();
        void _itsOffTime();
        bool _isTimeoutOffTime();
        unsigned int _get_brightness();
};