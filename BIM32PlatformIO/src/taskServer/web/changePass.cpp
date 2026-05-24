#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#include "../taskServer.hpp"
#include "../../config/config.hpp"

/**
 * Change password
 */
void TaskServer::_webChangePass() {
    if(_webIsLogged(true)) {
        String oldPass = (_server.hasArg("oldPass")) ? _server.arg("oldPass") : "";
        String newPass = (_server.hasArg("newPass")) ? _server.arg("newPass") : "";
        String res = "-";
        if(oldPass == config.account.pass()) {
            String json = "{\"pass\":\"";
            json += newPass;
            json += "\"}";
            File file = LittleFS.open("/user.us", FILE_WRITE);
            if(file.print(json)) res = "OK";
            else res = "ERROR Write file";
            file.close();
        }
        else res = "ERROR";
        _server.send(200, "text/plain", res);
    }
}