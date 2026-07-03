#include <Arduino.h>

#include "./taskSensors.hpp"
#include "../state/state.hpp"
#include "../sensors/sensors.hpp"
#include "../wsensor/wirelessSensor.hpp"
#include "../sound/sound.hpp"
#include "../comfort/comfort.hpp"

TaskSensors::TaskSensors() {}

TaskSensors::~TaskSensors() {
    if(_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
}

bool TaskSensors::start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId) {
    BaseType_t result = xTaskCreatePinnedToCore(TaskSensors::_taskWrapper, name, stackSize, this, priority, &_taskHandle, coreId);
    return (result == pdPASS);
}

void TaskSensors::_taskWrapper(void* pvParameters) {
    TaskSensors* instance = static_cast<TaskSensors*>(pvParameters);
    instance->_run(); 
}

void TaskSensors::_run() {
    sensors.init();

    while(1) {
        if(millis() - _sensorsTHP_update > 5000) {
            _sensorsTHP_update = millis();

            sensors.readTHP();

            comfort.calculate();
            comfort.devicesControl();
        }

        if(millis() - _sensorsL_update > 300) {
            _sensorsL_update = millis();

            sensors.readL();
        }

        sensors.BME680Read();

        wsensor.receive();

        comfort.soundNotify();
        sound.hourlySignal();
        sound.alarm();

        vTaskDelay(50);
    }
}