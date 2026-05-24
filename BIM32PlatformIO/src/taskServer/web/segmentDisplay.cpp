#include <Arduino.h>

#include "../taskServer.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

/**
 * Display timeslot color adjust
 */
void TaskServer::_webColor() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("hex") and _server.hasArg("slot") and _server.hasArg("num")) {
            char color[7];
            _server.arg("hex").toCharArray(color, 7);
            config.display.timeSlot.setColor(color, (_server.arg("slot")).toInt(), (_server.arg("num")).toInt());
            state.colorChanged = true;
            _server.send(200, "text/plain", "OK");
        }
        else _server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Display change animation
 */
void TaskServer::_webAnimation() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("num")) {
            int dispNum = _server.arg("num").toInt();
            if(_server.hasArg("type")) config.display.animation.setType(_server.arg("type").toInt(), dispNum);
            if(_server.hasArg("speed")) config.display.animation.setSpeed(_server.arg("speed").toInt(), dispNum);
            if(_server.hasArg("points")) config.display.animation.setPoints(_server.arg("points").toInt(), dispNum);
            _server.send(200, "text/plain", "OK");
        }
        _server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Show reassigned digits on 7-segment display
 */
void TaskServer::_webShowOrder() {
    if(_webIsLogged(true)) {
        if(_server.hasArg("order") && _server.hasArg("showOrder") && _server.hasArg("num")) {
            uint8_t num = (_server.arg("num")).toInt();
            uint8_t showOrder = (_server.arg("showOrder")).toInt();
            String order = _server.arg("order");
            Serial.println(order);
            int arr[16];
            int count = 0;
            char buf[64];
            order.toCharArray(buf, sizeof(buf));
            char *p = strtok(buf, ",");
            while(p != NULL) {
                arr[count++] = atoi(p);
                p = strtok(NULL, ",");
            }
            // SHOW_ORDER(num, showOrder, arr);
        }
        else _server.send(200, "text/plain", "error");
    }
}