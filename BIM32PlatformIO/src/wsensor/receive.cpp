#include "./wirelessSensor.hpp"
#include "../globals.hpp"

void WirelessSensor::receive() {
    while(Serial2.available() > 0) {
        char c = Serial2.read();
        
        if(_receivedIndex < (sizeof(_receivedData) - 2)) {
            _receivedData[_receivedIndex++] = c;
            _receivedData[_receivedIndex] = '\0';
        }
        else {
            _receivedIndex = 0; 
            _receivedData[0] = '\0';
        }
    }

    if(_receivedIndex == 0) return;

    char lastChar = _receivedData[_receivedIndex - 1];
    char currPrevChar = (_receivedIndex > 1) ? _receivedData[_receivedIndex - 2] : '\0';

    if(lastChar == '\n' || lastChar == '}' || currPrevChar == '\n' || currPrevChar == '}') {
        if(wirelessSensorQueue != NULL) {
            WsensorPacket packet;
            strlcpy(packet.data, _receivedData, sizeof(packet.data));
            xQueueSend(wirelessSensorQueue, &packet, 0);
            Serial.printf("Wireless Sensor received: %s\n", packet.data);
        }

        _receivedIndex = 0;
        _receivedData[0] = '\0';
    }
}