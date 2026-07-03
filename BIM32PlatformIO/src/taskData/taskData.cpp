#include <Arduino.h>

#include "./taskData.hpp"
#include "../globals.hpp"
#include "../pinout.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../sensors/sensors.hpp"
#include "../sound/sound.hpp"
#include "../network/network.hpp"
#include "../comfort/comfort.hpp"
#include "../wsensor/wirelessSensor.hpp"

TaskData::TaskData() {}

TaskData::~TaskData() {
    if(_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
}

bool TaskData::start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId) {
    BaseType_t result = xTaskCreatePinnedToCore(TaskData::_taskWrapper, name, stackSize, this, priority, &_taskHandle, coreId);
    return (result == pdPASS);
}

void TaskData::_taskWrapper(void* pvParameters) {
    TaskData* instance = static_cast<TaskData*>(pvParameters);
    instance->_run(); 
}

void TaskData::_run() {
    attachInterruptArg(DISPLAY1_BUTTON_PIN, isr_display1, this, FALLING);
    attachInterruptArg(DISPLAY2_BUTTON_PIN, isr_display2, this, FALLING);
    attachInterruptArg(ALARM_BUTTON_PIN, isr_alarm, this, FALLING);
    attachInterruptArg(MP3_BUSY_PIN, isr_mp3, this, FALLING);

    mqtt.init();

    network.scanNetworks();

    while(1) {
        if(digitalRead(SETTINGS_BUTTON_PIN) == 0) network.runAccessPoint(); // Enter access point mode if "Settings" button is pressed

        wsensor.parse();
        _networkConnect();
        
        if(!state.apMode) {
            _timeSyncNTP();
            _receiveThingspeak();
            _sendThingspeak();
            _sendNarodmon();
            _sendMqtt();
            _updateWeather();
            _updateHistoryRepository();
        }

        network.setNeedToPing();

        vTaskDelay(50);
    }
}