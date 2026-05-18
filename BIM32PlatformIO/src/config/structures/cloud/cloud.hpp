#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"
#include "./thingspeak.hpp"
#include "./narodmon.hpp"
#include "./mqtt.hpp"

class Config;

struct CloudStruct {
    public:
        ThinspeakReceiveStruct thingspeakReceive;
        ThingspeakSendStruct thingspeakSend;
        ThingspeakSendStruct history;
        NarodmonSendStruct narodmonSend;
        MQTTSendStruct mqttSend;
};