#include <Arduino.h>

#include "../taskServer.hpp"
#include "../../config/config.hpp"

/**
 * Change language
 */
void TaskServer::_webChangeLang() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("lang")) {
            config.set_lang(_server.arg("lang"));
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "error");
    }
}