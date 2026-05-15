#include <Arduino.h>
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties
#include <HTTPClient.h>
#include "./thingspeak.hpp"

#include <TimeLib.h> // v1.6.1 https://playground.arduino.cc/Code/Time/
#include "../../time/time.hpp"
#include "../../config.hpp"
#include "../../state.hpp"

/**
 * Receive data from weather history repository
 */
void Thingspeak::receiveHistory() {
    if(config.cloud.history.channelID() == "") {
        Serial.println("No Channel ID");
        return;
    }
    if(config.cloud.history.rdkey() == "") {
        Serial.println("No Read API Key");
        return;
    }
    String url = "http://api.thingspeak.com/channels/" + String(config.cloud.history.channelID());
    url += "/feeds.json?api_key=" + String(config.cloud.history.rdkey()) + "&results=24";
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
        for(int i=0; i<24; i++) {
            state.thing.historyData[0][i] = root["feeds"][i]["field1"].isNull() ? -99.0 : root["feeds"][i]["field1"];
            state.thing.historyData[1][i] = root["feeds"][i]["field2"].isNull() ? -99.0 : root["feeds"][i]["field2"];
            state.thing.historyData[2][i] = root["feeds"][i]["field3"].isNull() ? -99.0 : root["feeds"][i]["field3"];
            state.thing.historyData[3][i] = root["feeds"][i]["field4"].isNull() ? -99.0 : root["feeds"][i]["field4"];
            state.thing.historyData[4][i] = root["feeds"][i]["field5"].isNull() ? -99.0 : root["feeds"][i]["field5"];
            state.thing.historyData[5][i] = root["feeds"][i]["field6"].isNull() ? -99.0 : root["feeds"][i]["field6"];
            state.thing.historyData[6][i] = root["feeds"][i]["field7"].isNull() ? -99.0 : root["feeds"][i]["field7"];

            String thing_tm  = root["feeds"][i]["created_at"];
            TimeElements tmth;
            char buf[22];
            thing_tm.toCharArray(buf, 22);
            tmth.Year = atoi(strtok(buf, "-")) - 1970;
            tmth.Month = atoi(strtok(NULL, "-"));
            tmth.Day = atoi(strtok(NULL, "T"));
            tmth.Hour = atoi(strtok(NULL, ":"));
            tmth.Minute = atoi(strtok(NULL, ":"));
            tmth.Second = atoi(strtok(NULL, ":"));
            state.thing.historyTime[i] = makeTime(tmth);
            state.thing.historyTime[i] += config.clock.utc() * 3600;
            state.thing.historyTime[i] += config.clock.dlst() ? time.isSummertime() ? 3600 : 0 : 0;
        }
        Serial.println("successfull");
    }
    else Serial.println("error, code: " + String(httpCode));
    client.end();
}