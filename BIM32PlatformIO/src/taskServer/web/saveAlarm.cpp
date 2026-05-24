#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include "../taskServer.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

/**
 * Save alarms data to alarm file
 */
void TaskServer::_webSaveAlarm() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("alarm")) {
            String alr = _server.arg("alarm");
            if(alr.length() > 0) {
                if(alr.lastIndexOf("\"states\":") != -1) {
                    bool err = true;
                    File file = LittleFS.open("/alarm.json", "w");
                    if(file) err = !file.print(alr);
                    file.close();
                    state.filesystem.fsInfoUpdate = true;
                    _server.send(200, "text/plain", err ? "SAVE ERROR" : "OK");
                }
                else _server.send(200, "text/plain", "ALARM ERROR");
            }
            else _server.send(200, "text/plain", "ALARM ARGUMENT EMPTY");
            config.readConfig();
        }
        else _server.send(200, "text/plain", "NO ALARM ARGUMENT");
    }
}