#pragma once
#include <Arduino.h>
#include <HTTPClient.h>

#include "../../agregateData/sendData/agregateSendData.hpp"

class Narodmon {
    public: 
        void send();
    
    private:
        AgregateSendData agregateSendData;
        WiFiClient client; 
        String _fieldsPrepare(unsigned int fieldNum, String metrics, String mac);
};