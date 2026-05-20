#include "./taskDisplay.hpp"

TaskDisplay::TaskDisplay() {}

TaskDisplay::~TaskDisplay() {
    if(_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
}

bool TaskDisplay::start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId, int dispNum) {
    _dispNum = dispNum;

    BaseType_t result = xTaskCreatePinnedToCore(
        TaskDisplay::_taskWrapper,
        name,
        stackSize,
        this,
        priority,
        &_taskHandle,
        coreId
    );
    
    return (result == pdPASS);
}

void TaskDisplay::_taskWrapper(void* pvParameters) {
    TaskDisplay* instance = static_cast<TaskDisplay*>(pvParameters);
    instance->_run(); 
}

void TaskDisplay::_run() {
    while(true) {
        if(_dispNum == 1) {
            // Код для Дисплея 1
        } 
        else if(_dispNum == 2) {
            // Код для Дисплея 2
        }

        // ......

        vTaskDelay(10); 
    }
}
