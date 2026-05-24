#include <Arduino.h>

#include "../taskServer.hpp"
#include "../../network/network.hpp"

/**
 * Scan available networks 
 */
void TaskServer::_webNetlist() {
    if(_webIsLogged(true)) {
        network.scanNetworks();
        _server.send(200, "text/plain", "OK");
    }
}