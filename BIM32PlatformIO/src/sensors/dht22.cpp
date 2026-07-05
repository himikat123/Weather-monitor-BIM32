#include "./sensors.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../pinout.hpp"

void Sensors::_DHT22Init(void) {
    rmt_config_t rmt_rx;
    rmt_rx.rmt_mode = RMT_MODE_RX;
    rmt_rx.channel = RMT_CHANNEL_3;
    rmt_rx.gpio_num = (gpio_num_t)DHT22_PIN;
    rmt_rx.clk_div = 80;
    rmt_rx.mem_block_num = 1;
    rmt_rx.rx_config.filter_en = true;
    rmt_rx.rx_config.filter_ticks_thresh = 10;
    rmt_rx.rx_config.idle_threshold = 1000; 

    if(rmt_config(&rmt_rx) == ESP_OK) {
        if(rmt_driver_install(rmt_rx.channel, 1000, 0) == ESP_OK) {
            _dht22_det = true;
        }
    }

    float t = 0, h = 0;
    if(_readDht22Rmt(&t, &h)) _dht22_det = true;
    else {
        vTaskDelay(pdMS_TO_TICKS(500));
        if(_readDht22Rmt(&t, &h)) _dht22_det = true;
        else _dht22_det = false;
    }
}

void Sensors::_DHT22Read(void) {
    if(_dht22_det) {
        float currentTemp = 40400.0;
        float currentHum = 40400.0;

        if(_readDht22Rmt(&currentTemp, &currentHum)) {
            state.dht22.temp = currentTemp;
            state.dht22.hum = currentHum;
            state.dht22.updated = true;
        }
        else {
            state.dht22.temp = 40400.0;
            state.dht22.hum = 40400.0;
        }
    }
    else {
        state.dht22.temp = 40400.0;
        state.dht22.hum = 40400.0;
    }
}

bool Sensors::_readDht22Rmt(float *targetTemp, float *targetHum) {
    pinMode(DHT22_PIN, OUTPUT);
    digitalWrite(DHT22_PIN, LOW);
    delayMicroseconds(18000);
    
    digitalWrite(DHT22_PIN, HIGH);
    delayMicroseconds(30);

    pinMode(DHT22_PIN, INPUT_PULLUP);
    rmt_set_gpio(RMT_CHANNEL_3, RMT_MODE_RX, (gpio_num_t)DHT22_PIN, false);

    RingbufHandle_t rmt_rx_ringbuf = NULL;
    rmt_get_ringbuf_handle(RMT_CHANNEL_3, &rmt_rx_ringbuf);

    if(rmt_rx_ringbuf != NULL) {
        size_t dummy_size = 0;
        void* dummy_item = nullptr;
        while ((dummy_item = xRingbufferReceive(rmt_rx_ringbuf, &dummy_size, 0)) != nullptr) {
            vRingbufferReturnItem(rmt_rx_ringbuf, dummy_item);
        }
    }

    rmt_rx_start(RMT_CHANNEL_3, true);

    size_t rx_size = 0;
    rmt_item32_t* item = (rmt_item32_t*)xRingbufferReceive(rmt_rx_ringbuf, &rx_size, pdMS_TO_TICKS(5));
    rmt_rx_stop(RMT_CHANNEL_3);

    if(item == nullptr || rx_size < (40 * sizeof(rmt_item32_t))) {
        if(item != nullptr) vRingbufferReturnItem(rmt_rx_ringbuf, (void*)item);
        return false;
    }

    size_t num_items = rx_size / sizeof(rmt_item32_t);
    uint8_t bytes[5] = {0, 0, 0, 0, 0};
    int bit_idx = 0;

    for(size_t i=2; i<num_items && bit_idx<40; i++) {
        uint32_t high_duration = item[i].duration1;
        if(high_duration > 50) {
            bytes[bit_idx / 8] |= (1 << (7 - (bit_idx % 8)));
        }
        bit_idx++;
    }

    vRingbufferReturnItem(rmt_rx_ringbuf, (void*)item);

    if(bit_idx < 40) return false;

    uint8_t checksum = (bytes[0] + bytes[1] + bytes[2] + bytes[3]) & 0xFF;
    if(bytes[4] != checksum) return false;

    int16_t rawHum = (bytes[0] << 8) | bytes[1];
    int16_t rawTemp = (bytes[2] << 8) | bytes[3];

    if(rawTemp & 0x8000) rawTemp = -(rawTemp & 0x7FFF);

    *targetHum = (float)rawHum / 10.0f;
    *targetTemp = (float)rawTemp / 10.0f;

    return true;
}

float Sensors::get_dht22_temp() {
    return state.dht22.temp + config.sensors.dht22.tempCorr();
}

float Sensors::get_dht22_hum() {
    return state.dht22.hum + config.sensors.dht22.humCorr();
}