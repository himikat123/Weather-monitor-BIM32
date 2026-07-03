#pragma once

#include "./timeNTP/timeNTP.hpp"
#include "./cloud/mqtt/mqtt.hpp"
#include "./cloud/narodmon/narodmon.hpp"

class TaskData {
    public:
        TaskData();
        ~TaskData();

        bool start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);

    private:
        TaskHandle_t _taskHandle = nullptr;
        static void _taskWrapper(void* pvParameters);
        void _run();

        TimeNTP timeNTP;
        MQTT mqtt;
        Narodmon narodmon;

        unsigned int _ntp_update = 0;
        unsigned int _thingspeakReceive = 0;
        unsigned int _thingspeakSend = 0;
        unsigned int _narodmonSend = 0;
        unsigned int _mqttSend = 0;
        unsigned int _historyUpdate = 0;

        static void IRAM_ATTR isr_display1(void* arg);
        static void IRAM_ATTR isr_display2(void* arg);
        static void IRAM_ATTR isr_alarm(void* arg);
        static void IRAM_ATTR isr_mp3(void* arg);

        void IRAM_ATTR display1_toggle();
        void IRAM_ATTR display2_toggle();
        void IRAM_ATTR alarm_button();
        void IRAM_ATTR mp3_busy();

        void _networkState();
        void _networkConnect();
        void _timeSyncNTP();
        void _receiveThingspeak();
        void _sendThingspeak();
        void _sendNarodmon();
        void _sendMqtt();
        void _updateWeather();
        void _updateHistoryRepository();
};