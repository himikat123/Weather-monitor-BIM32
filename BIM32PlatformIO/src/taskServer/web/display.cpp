#include <Arduino.h>

#include "../taskServer.hpp"
#include "../../state/state.hpp"

/**
 * Turn display on and off
 */
void TaskServer::_webDispToggle() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("num")) {
            uint8_t dispNum = (_server.arg("num")).toInt();
            state.display_btn_pressed[dispNum] = true;
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "ERROR");
    }
}

/*
 * ILI9341 display touch calibrate
 */
void TaskServer::_webTouchCalibrate() {
    if(_webIsLogged(true)) {
        state.touch_calibrate = true;
        _server.send(200, "text/plain", "OK");
    }
}