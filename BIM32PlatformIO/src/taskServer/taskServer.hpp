#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "FS.h"
#include <LittleFS.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include "ESP32SSDP.h"

#include "../websocket/websocket.hpp"
#include "../globals.hpp"

class TaskServer {
    public:
        TaskServer();
        ~TaskServer();

        bool start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId);

    private:
        TaskHandle_t _taskHandle = nullptr;
        static void _taskWrapper(void* pvParameters);
        void _run();

        WiFiClient client;
        WebServer _server;
        WebSocket _websocket;
        File fsUploadFile;

        unsigned long _ws_update = 0;
        String _web_filelist = "";
        size_t _fsUsed = 0;
        size_t _fsTotal = 0;
        size_t content_len = 0;

        template<typename T>
        void updateIfNeeded(T& sensor, JsonObject& root) {
            if(sensor.updated) {
                sensor.toJson(root);
                sensor.updated = false;
            }
        }

        void _wsData();
        void _fsInfoUpdate();
        void _appendToFsList(const char* text);
        void _listAllFilesInDir(const char* dirname);
        String _webTimeString(unsigned int tm);
        String _webGetContentType(String filename);
        bool _webFileRead(String path);
        bool _webGetFile(String uri);
        void _webSens();
        bool _webIsLogged(bool answer);
        void _webLogin();
        void _webSave();
        void _webSaveAlarm();
        void _webRestart();
        void _webNetlist();
        void _webChangeLang();
        void _webDispToggle();
        void _webBrightLimit();
        void _webBright();
        void _webSensitivity();
        void _webAnimation();
        void _webColor();
        void _webShowOrder();
        void _webTouchCalibrate();
        void _webSyncClock();
        void _webSyncdialog();
        void _webChangePass();
        void _webSoundVolume();
        void _webSoundEqualizer();
        void _webSoundPlay();
        void _webSoundStop();
        void _webDefault();
        void _webDebugTouch();
        void _webDebugWeather();
        void _webFileUpload();
        void _webFileDelete();
        void _webFileRename();
};