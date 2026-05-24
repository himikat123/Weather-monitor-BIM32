#include <Arduino.h>

#include "../taskServer.hpp"
#include "../../state/state.hpp"

void TaskServer::_webDebugTouch() {
    state.debugTouch = true;
    _server.send(200, "text/plain", "OK");
}

void TaskServer::_webDebugWeather() {
    state.debugWether = true;
    _server.send(200, "text/plain", "OK");
}