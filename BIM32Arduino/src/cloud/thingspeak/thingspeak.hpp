#pragma once
#include <Arduino.h>

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
        String _fieldPrepare(unsigned int field);
        String _historyFieldPrepare(unsigned int fieldNum);
        boolean _is_summertime();
};

extern Thingspeak thingspeak;