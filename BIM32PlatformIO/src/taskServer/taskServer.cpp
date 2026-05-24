#include <Arduino.h>

#include "./taskServer.hpp"
#include "./sensors/sensors.hpp"

TaskServer::TaskServer() : 
    _server(80), 
    _websocket(88, 1) 
{ }

TaskServer::~TaskServer() {
    if(_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
}

bool TaskServer::start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId) {
    _server.on("/data.json",         HTTP_GET,  [this]() { _webSens(); });
    _server.on("/esp/login",         HTTP_POST, [this]() { _webLogin(); });
    _server.on("/esp/saveConfig",    HTTP_POST, [this]() { _webSave(); });
    _server.on("/esp/saveAlarm",     HTTP_POST, [this]() { _webSaveAlarm(); });
    _server.on("/esp/restart",       HTTP_GET,  [this]() { _webRestart(); });
    _server.on("/esp/netlist",       HTTP_GET,  [this]() { _webNetlist(); });
    _server.on("/esp/changelang",    HTTP_GET,  [this]() { _webChangeLang(); });
    _server.on("/esp/dispToggle",    HTTP_GET,  [this]() { _webDispToggle(); });
    _server.on("/esp/brightLimit",   HTTP_GET,  [this]() { _webBrightLimit(); });
    _server.on("/esp/bright",        HTTP_GET,  [this]() { _webBright(); });
    _server.on("/esp/sensitivity",   HTTP_GET,  [this]() { _webSensitivity(); });
    _server.on("/esp/animation",     HTTP_GET,  [this]() { _webAnimation(); });
    _server.on("/esp/color",         HTTP_GET,  [this]() { _webColor(); });
    _server.on("/esp/showOrder",     HTTP_GET,  [this]() { _webShowOrder(); });
    _server.on("/esp/calibrate",     HTTP_GET,  [this]() { _webTouchCalibrate(); });
    _server.on("/esp/syncClock",     HTTP_GET,  [this]() { _webSyncClock(); });
    _server.on("/esp/syncdialog",    HTTP_GET,  [this]() { _webSyncdialog(); });
    _server.on("/esp/changePass",    HTTP_POST, [this]() { _webChangePass(); });
    _server.on("/esp/volume",        HTTP_GET,  [this]() { _webSoundVolume(); });
    _server.on("/esp/equalizer",     HTTP_GET,  [this]() { _webSoundEqualizer(); });
    _server.on("/esp/mp3play",       HTTP_GET,  [this]() { _webSoundPlay(); });
    _server.on("/esp/mp3stop",       HTTP_GET,  [this]() { _webSoundStop(); });
    _server.on("/esp/defaultConfig", HTTP_POST, [this]() { _webDefault(); });
    _server.on("/debug/touch",       HTTP_GET,  [this]() { _webDebugTouch(); });
    _server.on("/debug/weather",     HTTP_GET,  [this]() { _webDebugWeather(); });
    _server.on("/esp/delete",        HTTP_POST, [this]() { _webFileDelete(); });
    _server.on("/esp/rename",        HTTP_POST, [this]() { _webFileRename(); });
    _server.on("/esp/fileUpload",    HTTP_POST, [this]() { _server.send(200, "text/plain", ""); }, [this]() { _webFileUpload(); });
    _server.onNotFound([this]() { if(!_webGetFile(_server.uri())) { _server.send(404, "text/plain", "FileNotFound"); }});
    _server.on("/description.xml",   HTTP_GET, [this]() { WebServer& server = _server; SSDP.getSchema(); });

    MDNS.begin("bim32");

    _server.begin();
    _websocket.begin();

    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    SSDP.setSchemaURL("description.xml");
    SSDP.setHTTPPort(80);
    SSDP.setName("Weather monitor BIM32");
    SSDP.setSerialNumber(chipId);
    SSDP.setURL("/");
    SSDP.setModelName("BIM32");
    SSDP.setModelNumber(FW); // FW должен быть виден (из globals или config)
    SSDP.setModelURL("https://radiokot.ru/artfiles/6571/");
    SSDP.setManufacturer("himikat123@gmail.com");
    SSDP.setManufacturerURL("https://github.com/himikat123/Weather-monitor-BIM32");
    SSDP.setDeviceType("rootdevice");
    SSDP.setServerName("SSDPServer/1.0");
    SSDP.setIcons(
        "<icon>"
            "<mimetype>image/png</mimetype>"
            "<height>48</height>"
            "<width>48</width>"
            "<depth>32</depth>"
            "<url>icon48.png</url>"
        "</icon>");
    SSDP.begin();

    MDNS.addService("http", "tcp", 80);

    BaseType_t result = xTaskCreatePinnedToCore(TaskServer::_taskWrapper, name, stackSize, this, priority, &_taskHandle, coreId);
    return (result == pdPASS);
}

void TaskServer::_taskWrapper(void* pvParameters) {
    TaskServer* instance = static_cast<TaskServer*>(pvParameters);
    instance->_run(); 
}

void TaskServer::_run() {
    while(1) {
        _server.handleClient();
        _websocket.loop();

        if(_websocket.hasClient()) {
            if(millis() - _ws_update >= 1000) {
                _ws_update = millis();
                _wsData();
            }
        }

        _fsInfoUpdate();

        if(sensorsSemaphore != NULL) {
            if(xSemaphoreTake(sensorsSemaphore, (TickType_t)100) == pdTRUE) {
                sensors.BME680Read();
                xSemaphoreGive(sensorsSemaphore);
            }
        }
        
        vTaskDelay(10);
    }
}