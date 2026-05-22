#include <Arduino.h>

#include "../globals.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
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
    int type = config.display.type(_dispNum);
    int model = config.display.model(_dispNum);
    bool isDisp1 = _dispNum == DISPLAY_1;

    switch(type) {
        case LCD_DISPLAY:
            if(model <= D_NX4827K043) _display = &nextion;
            if(model == D_ILI9341) _display = &ili9341;
            break;
        case PIXEL_LEDS_DISPLAY: _display = isDisp1 ? &_ws2812b_1 : &_ws2812b_2; break;
        case SEGMENT_DISPLAY:
            if(config.display.model(_dispNum) <= D_TM1637) _display = isDisp1 ? &_tm1637_1 : &_tm1637_2;
            if(config.display.model(_dispNum) >= D_MAX7219) _display = isDisp1 ? &_max7219s_1 : &_max7219s_2;
            break;
        case NUMITRON_DISPLAY: _display = isDisp1 ? &_numitron_1 : &_numitron_2; break;
        default: ;;
    }

    BaseType_t result = xTaskCreatePinnedToCore(TaskDisplay::_taskWrapper, name, stackSize, this, priority, &_taskHandle, coreId);
    return (result == pdPASS);
}

void TaskDisplay::_taskWrapper(void* pvParameters) {
    TaskDisplay* instance = static_cast<TaskDisplay*>(pvParameters);
    instance->_run(); 
}

void TaskDisplay::_run() {
    _initDisplay();

    while(true) {
        if(state.display_btn_pressed[_dispNum]) {
            state.display_btn_pressed[_dispNum] = false;
            state.disp_autoOff[_dispNum] = millis();
            _display->displayToggle();
            vTaskDelay(500);
        }
        _display->refresh();
        _brightnessAndDotFreq();
        _itsOffTime();

        if(config.display.type(_dispNum) == LCD_DISPLAY) {
            int model = config.display.model(_dispNum);

            /* Receive data from Nextion */
            if(model == D_NX4832K035 or model == D_NX4832T035 or model == D_NX4827K043) nextion.dataReceive();

            /* Get touch clicks on ILI9341 */
            if(model == D_ILI9341) ili9341.getTouch();
        }

        vTaskDelay(10); 
    }
}
