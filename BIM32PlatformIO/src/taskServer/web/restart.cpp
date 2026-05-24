#include <Arduino.h>

#include "../taskServer.hpp"

/**
 * Restart ESP32
 */
void TaskServer::_webRestart() {
    if(_webIsLogged(true)) {
        _server.send(200, "text/plain", "OK");
        ESP.restart();
    }
}