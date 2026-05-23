#include <Arduino.h>

#include "./taskSensors.hpp"
#include "../sensors/sensors.hpp"

void TaskSensors::_updateTempHumPresSensors() {
    if(sensorsSemaphore != NULL) {
        if(xSemaphoreTake(sensorsSemaphore, (TickType_t)100) == pdTRUE) {
            sensors.readTHP();
            xSemaphoreGive(sensorsSemaphore);
        }
    }
}

void TaskSensors::_updateLightSensors() {
    if(sensorsSemaphore != NULL) {
        if(xSemaphoreTake(sensorsSemaphore, (TickType_t)100) == pdTRUE) {
            sensors.readL();
            xSemaphoreGive(sensorsSemaphore);
        }
    }
}