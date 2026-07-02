#pragma once

#include "../comfort/comfort.hpp"

class TaskSensors {
    public:
        TaskSensors();
        ~TaskSensors();

        bool start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);

    private:
        TaskHandle_t _taskHandle = nullptr;
        static void _taskWrapper(void* pvParameters);
        void _run();

        Comfort comfort;

        unsigned int _sensorsTHP_update = 0;
        unsigned int _sensorsL_update = 0;

        void _updateTempHumPresSensors();
        void _updateLightSensors();
};