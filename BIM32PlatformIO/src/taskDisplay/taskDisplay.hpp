#pragma once
#include <Arduino.h>

class TaskDisplay {
    public:
        TaskDisplay();
        ~TaskDisplay();

        bool start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId, int dispNum);

    private:
        TaskHandle_t _taskHandle = nullptr;
        static void _taskWrapper(void* pvParameters);
        void _run();

        int _dispNum = 0;

        bool _isNightOffTime();
        bool _isTimeoutOffTime();
        unsigned int _get_brightness();
};