#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include "../taskServer.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

/**
 * Save config data to config file
 */
void TaskServer::_webSave() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("config")) {
            String cfg = _server.arg("config");
            if(cfg.length() > 0) {
                if(cfg.lastIndexOf("\"ssid\":") != -1) {
                    bool err = true;
                    File file = LittleFS.open("/config.json", "w");
                    if(file) err = !file.print(cfg);
                    file.close();
                    state.filesystem.fsInfoUpdate = true;
                    _server.send(200, "text/plain", err ? "SAVE ERROR" : "OK");
                }
                else _server.send(200, "text/plain", "CONFIG ERROR");
            }
            else _server.send(200, "text/plain", "CONFIG ARGUMENT EMPTY");
            config.readConfig();
        }
        else _server.send(200, "text/plain", "NO CONFIG ARGUMENT");
    }
}