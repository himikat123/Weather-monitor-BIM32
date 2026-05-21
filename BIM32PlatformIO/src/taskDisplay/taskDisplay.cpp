#include <Arduino.h>

#include "../globals.hpp"
#include "../config/config.hpp"
#include "./taskDisplay.hpp"
#include "../displays/nextion/nextion.hpp"

TaskDisplay::TaskDisplay() {}

TaskDisplay::~TaskDisplay() {
    if(_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
}

bool TaskDisplay::start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId, int dispNum) {
    _dispNum = dispNum;

    if(config.display.type(_dispNum) == LCD_DISPLAY) {
        if(config.display.model(_dispNum) <= D_NX4827K043) _display = &nextion;
        if(config.display.model(_dispNum) == D_ILI9341) _display = &ili9341;
        if(config.display.type(_dispNum) == PIXEL_LEDS_DISPLAY) {
            _display = (_dispNum == DISPLAY_1) ? &_ws2812b_1 : &_ws2812b_2;
        }
        if(config.display.type(_dispNum) == SEGMENT_DISPLAY) {
            if(config.display.model(_dispNum) <= D_TM1637) {
                _display = (_dispNum == DISPLAY_1) ? &_tm1637_1 : &_tm1637_2;
            }
            if(config.display.model(_dispNum) >= D_MAX7219) {
                _display = (_dispNum == DISPLAY_1) ? &_max7219s_1 : &_max7219s_2;
            }
        }
        if(config.display.type(_dispNum) == NUMITRON_DISPLAY) {
            _display = (_dispNum == DISPLAY_1) ? &_numitron_1 : &_numitron_2;
        }
    }

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
