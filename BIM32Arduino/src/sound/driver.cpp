#include <Arduino.h>

#include "./sound.hpp"
#include "../pinout.hpp"
#include "../globals.hpp"
#include "../config/config.hpp"

void Sound::_reset() {
    _sendCommand(0x0C, 0x00, 0x00);
    _sendCommand(0x09, 0x00, 0x02);
    vTaskDelay(200);
    equalizer(config.sound.eq());
    vTaskDelay(100);
    volume(config.sound.vol());
    vTaskDelay(100);
}

uint16_t Sound::_chckSum(uint8_t *sdata) {
    uint16_t sum = 0;
    for(int i=1; i<7; i++) {
        sum += sdata[i];
    }
    return -sum;
}

void Sound::_sendCommand(uint8_t command, uint8_t hByte, uint8_t lByte) {
    uint8_t sdata[10] = {0x7E, 0xFF, 0x06, command, 0x00, hByte, lByte, 0x00, 0x00, 0xEF};
    uint16_t sum = _chckSum(sdata);
    sdata[7] = (uint8_t)(sum >> 8);
    sdata[8] = (uint8_t)(sum);
    if(state.uart2_tx != DFPlayer) {
        Serial2.setPins(HC12_RX_PIN, MP3_TX_PIN);
        state.uart2_tx = DFPlayer;
    }
    Serial2.write(sdata, 10);
    Serial2.flush();
}