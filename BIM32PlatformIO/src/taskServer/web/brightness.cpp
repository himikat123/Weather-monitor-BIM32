#include <Arduino.h>

#include "../taskServer.hpp"
#include "../../config/config.hpp"

/**
 * Change brightness limit
 */
void TaskServer::_webBrightLimit() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("min") and _server.hasArg("max") and _server.hasArg("num")) {
            config.display.brightness.setBrightLimit(
                (_server.arg("min")).toInt(), 
                (_server.arg("max")).toInt(), 
                (_server.arg("num")).toInt()
            );
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Brightness adjust
 */
void TaskServer::_webBright() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("bright") and _server.hasArg("num")) {
            config.display.brightness.setBright((_server.arg("bright")).toInt(), (_server.arg("num")).toInt());
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Ambient light sensor sensitivity adjust
 */
void TaskServer::_webSensitivity() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("bright") and _server.hasArg("num")) {
            config.display.lightSensor.setSensitivity((_server.arg("bright")).toInt(), (_server.arg("num")).toInt());
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "ERROR");
    }
}