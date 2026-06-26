#include <Arduino.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

#include "./taskServer.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../wsensor/wirelessSensor.hpp"

void TaskServer::_wsData() {
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();

    if(_websocket.available()) {
        String msg = _websocket.read();
        if(msg.length()) {
            doc["state"] = config.account.required() ? ((String(state.secretCode) == msg) ? "OK" : "LOGIN") : "OK";
            //Serial.println(String(state.secretCode));
            //Serial.println(msg);
        }
    }

    doc["runtime"] = esp_timer_get_time() / 1000000ULL;
    doc["heap"] = ESP.getFreeHeap();
    doc["time"] = now();

    JsonArray dispState = root.createNestedArray("dispState");
    dispState.add(state.disp_on_off[0] ? 1 : 0);
    dispState.add(state.disp_on_off[1] ? 1 : 0);

    if(state.wsensor.updated) doc["wsensRaw"] = String(wsensor.get_rawData());

    updateIfNeeded(state.network, root);
    updateIfNeeded(state.bme280, root);
    updateIfNeeded(state.bmp180, root);
    updateIfNeeded(state.sht21, root);
    updateIfNeeded(state.dht22, root);
    updateIfNeeded(state.esp32core, root);
    updateIfNeeded(state.max44009, root);
    updateIfNeeded(state.bh1750, root);
    updateIfNeeded(state.analog, root);
    updateIfNeeded(state.bme680, root);
    updateIfNeeded(state.thing, root);
    updateIfNeeded(state.weather, root);
    updateIfNeeded(state.wsensor, root);
    updateIfNeeded(state.filesystem, root);

    String data = "";
    serializeJson(root, data);
    _websocket.sendJson(data);
}