#pragma once
#include <Arduino.h>

#include "../../timeNTP/timeNTP.hpp"
#include "../../agregateData/sendData/agregateSendData.hpp"

class Thingspeak {
    public:
        static Thingspeak& getInstance() {
            static Thingspeak instance;
            return instance;
        }

        Thingspeak(const Thingspeak&) = delete;
        void operator=(const Thingspeak&) = delete;

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
        Thingspeak() {}

        AgregateSendData agregateSendData;
        TimeNTP timeNTP;
        String _fieldPrepare(unsigned int field);
        String _historyFieldPrepare(unsigned int fieldNum);
};

inline Thingspeak& thingspeak = Thingspeak::getInstance();