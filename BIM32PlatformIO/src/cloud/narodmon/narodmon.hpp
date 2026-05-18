#pragma once
#include <Arduino.h>

class Narodmon {
    public: void send();
    private: String _fieldsPrepare(unsigned int fieldNum, String metrics, String mac);
};