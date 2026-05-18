#pragma once
#include <Arduino.h>
#include <HTTPClient.h>

class Narodmon {
    public: 
        void send();
    
    private:
        WiFiClient client; 
        String _fieldsPrepare(unsigned int fieldNum, String metrics, String mac);
};