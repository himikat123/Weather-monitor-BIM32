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
        if(_server.hasArg("order") && _server.hasArg("num") && _server.hasArg("show")) {
            uint8_t num = (_server.arg("num")).toInt();
            uint8_t show = (_server.arg("show")).toInt();
            if(num == 0 || num == 1) {
                String order = _server.arg("order");
                //Serial.println(order);
                int count = 0;
                char buf[32];
                order.toCharArray(buf, sizeof(buf));
                char *p = strtok(buf, ",");
                while(p != NULL && count < 8) {
                    state.segmentDispOrder[num][count++] = atoi(p);
                    p = strtok(NULL, ",");
                }
                state.segmentDispOrder[num][8] = (show == 1) ? 1 : 0;
            }
        }
        else _server.send(200, "text/plain", "error");
    }
}