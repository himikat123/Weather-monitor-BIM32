#include <Arduino.h>

#include "./sound.hpp"
#include "../pinout.hpp"
#include "../globals.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"

void Sound::_initRMT() {
    rmt_config_t rmt_tx;
    rmt_tx.rmt_mode = RMT_MODE_TX;
    rmt_tx.channel = RMT_CHANNEL_4;
    rmt_tx.gpio_num = (gpio_num_t)MP3_TX_PIN;
    rmt_tx.mem_block_num = 1;
    rmt_tx.clk_div = 80;
    rmt_tx.tx_config.loop_en = false;
    rmt_tx.tx_config.carrier_en = false;
    rmt_tx.tx_config.idle_output_en = true;
    rmt_tx.tx_config.idle_level = RMT_IDLE_LEVEL_HIGH;
    rmt_config(&rmt_tx);
    rmt_driver_install(rmt_tx.channel, 0, 0);
}

void Sound::_reset() {
    _sendCommand(0x0C, 0x00, 0x00);
    _sendCommand(0x09, 0x00, 0x02);
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

    if(len > 10) len = 10; 
    rmt_item32_t items[100]; 

    size_t item_idx = 0;
    const uint32_t half_bit = 52;

    for(size_t i=0; i<len; i++) {
        uint8_t byte = data[i];

        items[item_idx++] = {{{ half_bit, 0, half_bit, 0 }}};

        for(int b=0; b<8; b++) {
            uint32_t level = (byte & (1 << b)) ? 1 : 0;
            items[item_idx++] = {{{ half_bit, level, half_bit, level }}};
        }

        items[item_idx++] = {{{ half_bit, 1, half_bit, 1 }}};
    }

    rmt_write_items(RMT_CHANNEL_4, items, item_idx, true);
}