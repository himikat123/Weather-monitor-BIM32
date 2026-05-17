#include <Arduino.h>
#include <HTTPClient.h>
#include "./thingspeak.hpp"

#include "../../config/config.hpp"

/**
 * Send data to thingspeak
 */
void Thingspeak::send() {
    if(config.cloud.thingspeakSend.wrkey() == "") {
        Serial.println("No Write API Key");
        return;
    }

    String url = "http://api.thingspeak.com/update?api_key=" + String(config.cloud.thingspeakSend.wrkey());
    for(unsigned int i=0; i<8; i++) {
        url += _fieldPrepare(i);
    }

    String httpData = "";
    HTTPClient client;
    //Serial.println(url);
    client.begin(url);
    int httpCode = client.GET();
    if(httpCode == HTTP_CODE_OK) {
        httpData = client.getString();
        //Serial.println(httpData);
        Serial.println("successfull");
    }
    else Serial.println("error, code: " + String(httpCode));
    client.end();
    httpData = "";
}