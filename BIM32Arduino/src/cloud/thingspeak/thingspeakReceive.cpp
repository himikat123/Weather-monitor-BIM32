#include <Arduino.h>
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include <HTTPClient.h>
#include "./thingspeak.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../../timeNTP/timeNTP.hpp"
#include "../../config/config.hpp"
#include "../../state/state.hpp"

/**
 * Receive data from thingspeak
 */
void Thingspeak::receive() {
    if(config.cloud.thingspeakReceive.channelID() == "") {
        Serial.println("No Channel ID");
        return;
    }
    if(config.cloud.thingspeakReceive.rdkey() == "") {
        Serial.println("No Read API Key");
        return;
    }
    String url = "http://api.thingspeak.com/channels/" + String(config.cloud.thingspeakReceive.channelID());
    url += "/feeds.json?api_key=" + String(config.cloud.thingspeakReceive.rdkey()) + "&results=1";
    String httpData = "";
    HTTPClient client;
    //Serial.println(url);
    client.begin(url);
    int httpCode = client.GET();
    if(httpCode == HTTP_CODE_OK) {
        httpData = client.getString();
        //Serial.println(httpData);
        JsonDocument root;
        DeserializationError error = deserializeJson(root, httpData);
        if(error) {
            Serial.println("Deserialization error");
            return;
        }
        String thing_tm  = root["feeds"][0]["created_at"];
        state.thing.data[0] = root["feeds"][0]["field1"];
        state.thing.data[1] = root["feeds"][0]["field2"];
        state.thing.data[2] = root["feeds"][0]["field3"];
        state.thing.data[3] = root["feeds"][0]["field4"];    
        state.thing.data[4] = root["feeds"][0]["field5"];
        state.thing.data[5] = root["feeds"][0]["field6"];
        state.thing.data[6] = root["feeds"][0]["field7"];
        state.thing.data[7] = root["feeds"][0]["field8"];
        TimeElements tmth;
        char buf[22];
        thing_tm.toCharArray(buf, 22);
        tmth.Year = atoi(strtok(buf, "-")) - 1970;
        tmth.Month = atoi(strtok(NULL, "-"));
        tmth.Day = atoi(strtok(NULL, "T"));
        tmth.Hour = atoi(strtok(NULL, ":"));
        tmth.Minute = atoi(strtok(NULL, ":"));
        tmth.Second = atoi(strtok(NULL, ":"));
        state.thing.time = makeTime(tmth);
        state.thing.time += config.clock.utc() * 3600;
        state.thing.time += config.clock.dlst() ? timeNTP.isSummertime() ? 3600 : 0 : 0;
        Serial.printf("successfully updated at %02d:%02d:%02d\r\n", hour(), minute(), second());
        state.thing.updated = true;
    }
    else Serial.println("error, code: " + String(httpCode));
    client.end();
}