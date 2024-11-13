#include <Wire.h>
#include <Adafruit_PCF8575.h>

#define DISP4    0
#define DISP6    1
#define DOT    100
#define SYMBOLS  0
#define WITH_DOT 1

class PCF8575_S : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t scl, int8_t sda);
        uint8_t refresh();

    private:
        Adafruit_PCF8575 _PCF1, _PCF2, _PCF3, _PCF4;
        Adafruit_PCF8575 _PCF[4] = { _PCF1, _PCF2, _PCF3, _PCF4 };
        int8_t _scl = -1;
        int8_t _sda = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        uint8_t _print();
        void _sendToDisplay(uint8_t bright);
        void _clearDisplay();
};

/**
 * Display initialize
 * @param dispNum display number
 * @param scl display SCL pin number
 * @param sda display SDA pin number
 */
void PCF8575_S::init(uint8_t dispNum, int8_t scl, int8_t sda) {
    _dispNum = dispNum;
    _scl = scl;
    _sda = sda;
    _setModel(config.display_model(dispNum));

    if(sda > 0 && scl > 0) {
        Wire1.begin(sda, scl, 400000);
        for(int i=0; i<4; i++) {
            _PCF[i].begin(0x20 + (dispNum * 4) + i, &Wire1);
        }
        vTaskDelay(100);

        _clearDisplay();
    }
}

/**
 * Displays the next display slot
 */
uint8_t PCF8575_S::refresh() {
    _slotSwitch();
    _segAnimations();
    return _print();
}

/**
 * Display data on the display
 */
uint8_t PCF8575_S::_print() {
    uint8_t bright = map(_brightness, 0, 100, 1, 10);
    bright = constrain(bright, 1, 10);
    bool updated = false;

    for(uint8_t i=0; i<8; i++) {
        if(_dispImg[i] >= DOT) _pixels[i] = font_pcf8575[WITH_DOT][_dispImg[i] - DOT];
        else _pixels[i] = font_pcf8575[SYMBOLS][_dispImg[i]];

        if(_pixelsPrev[i] != _pixels[i]) {
            _pixelsPrev[i] = _pixels[i];
            updated |= true;
        }
    }

    if(bright < 10) {
        _sendToDisplay(bright);
        return 10 - bright;
    }        
    if(updated) _sendToDisplay(bright);
 
    return bright;
}

/**
 * Send data to display
 */
void PCF8575_S::_sendToDisplay(uint8_t bright) {
    if(_sda > 0 && _scl > 0) {
        byte seg[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        if(_power) {
            for(uint8_t i=0; i<8; i++) seg[i] = _pixels[i];
        }

        for(int i=0; i<4; i++) {
            uint16_t buf = (uint16_t)seg[i * 2];
            buf = buf << 8;
            buf += (uint16_t)seg[i * 2 + 1];
            if(sensorsSemaphore != NULL) {
                if(xSemaphoreTake(sensorsSemaphore, (TickType_t)100) == pdTRUE) {
                    _PCF[i].digitalWriteWord(buf);
                    xSemaphoreGive(sensorsSemaphore);
                }
            }
        }

        if(bright < 10) {
            vTaskDelay(bright);
            for(int i=0; i<4; i++) {
                if(sensorsSemaphore != NULL) {
                    if(xSemaphoreTake(sensorsSemaphore, (TickType_t)100) == pdTRUE) {
                        _PCF[i].digitalWriteWord(0xFFFF);
                        xSemaphoreGive(sensorsSemaphore);
                    }
                }
            }
        }
    }
}

/**
 * Clear the display
 */
void PCF8575_S::_clearDisplay() {
    for(int i=0; i<8; i++) _pixels[i] = 0xFF;
    _print();
}