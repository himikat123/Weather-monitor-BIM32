#include <Arduino.h>

#include "./sound.hpp"
#include "../pinout.hpp"
#include "../globals.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

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

    _sendRmt(sdata, 10);
}

void Sound::_sendRmt(const uint8_t *data, size_t len) {
    if(len == 0 || data == nullptr) return;

    size_t total_bits = len * 10;
    
    rmt_item32_t* items = (rmt_item32_t*)malloc(total_bits * sizeof(rmt_item32_t));
    if(!items) return;

    size_t item_idx = 0;
    uint32_t bit_duration = 104;

    for(size_t i=0; i<len; i++) {
        uint8_t byte = data[i];
        items[item_idx++] = {{{ bit_duration, 0, 0, 0 }}};

        for(int b=0; b<8; b++) {
            uint32_t level = (byte & (1 << b)) ? 1 : 0;
            items[item_idx++] = {{{ bit_duration, level, 0, level }}};
        }

        items[item_idx++] = {{{ bit_duration, 1, 0, 1 }}};
    }

    rmt_write_items(RMT_CHANNEL_4, items, total_bits, false);
    free(items);
}