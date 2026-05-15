#pragma once
#include <Arduino.h>
#include "../../../globals.hpp"
#include "./thingspeak.hpp"
#include "./narodmon.hpp"
#include "./mqtt.hpp"

class Config;

struct Cloud {
    public:
        ThinspeakReceive thingspeakReceive;
        ThingspeakSend thingspeakSend;
        ThingspeakSend history;
        NarodmonSend narodmonSend;
        MQTTSend mqttSend;
};