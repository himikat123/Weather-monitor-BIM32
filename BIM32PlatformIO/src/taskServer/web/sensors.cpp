#include <Arduino.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

#include "../taskServer.hpp"
#include "../../globals.hpp"
#include "../../state/state.hpp"

/**
 * Send sensors data via AJAX
 */
void TaskServer::_webSens() {
    JsonDocument json;
    state.toJson(json);
    json["state"] = _webIsLogged(false) ? "OK" : "LOGIN";
    json["fw"] = FW;
    json["runtime"] = esp_timer_get_time() / 1000ULL;
    json["heap"] = ESP.getFreeHeap();
    json["time"] = now();

    JsonArray dispState = json.as<JsonObject>().createNestedArray("dispState");
    dispState.add(state.disp_on_off[0] ? 1 : 0);
    dispState.add(state.disp_on_off[1] ? 1 : 0);

    #if defined(BIM32_CYD)
        json["cyd"] = 1;
    #endif

    String data = "";
    serializeJson(json, data);
    _server.send(200, "application/json", data);
}