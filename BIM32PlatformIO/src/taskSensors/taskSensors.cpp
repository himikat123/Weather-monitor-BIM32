#include <Arduino.h>

#include "./taskSensors.hpp"
#include "../globals.hpp"
#include "../pinout.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../sensors/sensors.hpp"
#include "../sound/sound.hpp"
#include "../network/network.hpp"
#include "../comfort/comfort.hpp"
#include "../wsensor/wirelessSensor.hpp"

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
    attachInterruptArg(DISPLAY1_BUTTON_PIN, isr_display1, this, FALLING);
    attachInterruptArg(DISPLAY2_BUTTON_PIN, isr_display2, this, FALLING);
    attachInterruptArg(ALARM_BUTTON_PIN, isr_alarm, this, FALLING);
    attachInterruptArg(MP3_BUSY_PIN, isr_mp3, this, FALLING);

    sensors.init();
    _HC12channelNrRequest();
    sound.init();
    mqtt.init();

    network.scanNetworks();

    while(1) {
        if(digitalRead(SETTINGS_BUTTON_PIN) == 0) network.runAccessPoint(); // Enter access point mode if "Settings" button is pressed

        if(millis() - _sensorsTHP_update > 5000) {
            _sensorsTHP_update = millis();
            _updateTempHumPresSensors();
            comfort.calculate();
            comfort.devicesControl();
            _networkState();
        }

        if(millis() - _sensorsL_update > 300) {
            _sensorsL_update = millis();
            _updateLightSensors();
        }

        wsensor.receive();
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

        comfort.soundNotify();
        sound.hourlySignal();
        sound.alarm();
        network.setNeedToPing();

        vTaskDelay(50);
    }
}