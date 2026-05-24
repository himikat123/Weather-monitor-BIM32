#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include "../taskServer.hpp"
#include "../../state/state.hpp"

/**
 * Restore default config
 */
void TaskServer::_webDefault() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("config") && _server.arg("config") == "default") {
            char ibuffer[64];
            if(LittleFS.exists("/config.json") == true) LittleFS.remove("/config.json");
            File def = LittleFS.open("/defaultConfig.json", "r");
            File cfg = LittleFS.open("/config.json", "w");
            if(def and cfg) {
                while(def.available()) {
                    byte i = def.readBytes(ibuffer, 64);
                    cfg.write((uint8_t *)ibuffer, i);
                }
                _server.send(200, "text/plain", "OK");
            }
            cfg.close();
            def.close();
            state.filesystem.fsInfoUpdate = true;
            _server.send(200, "text/plain", "error");
        }
        else _server.send(200, "text/plain", "bad args");
    }
}