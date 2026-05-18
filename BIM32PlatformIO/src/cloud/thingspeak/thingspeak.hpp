#pragma once
#include <Arduino.h>

#include "../../timeNTP/timeNTP.hpp"

class Thingspeak {
    public:
        void receive();
        void send();
        void sendHistory();
        void receiveHistory();
        bool dataRelevance();
        float get_field(unsigned int num);
        time_t get_updated();
        float get_historyField(unsigned int sensor, unsigned int slot);
        time_t get_historyUpdated(unsigned int slot);

    private:
        TimeNTP timeNTP;
        String _fieldPrepare(unsigned int field);
        String _historyFieldPrepare(unsigned int fieldNum);
};

extern Thingspeak thingspeak;