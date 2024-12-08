#include <Wire.h>
#include <Adafruit_PCF8575.h>

#define DISP4    0
#define DISP6    1
#define DOT    100
#define SYMBOLS  0
#define WITH_DOT 1

class PCF8575_S : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t scl, int8_t sda, int8_t pwm, int8_t ws);
        void brightness(uint8_t intensity, bool reduc);
        void refresh();

    private:
        Adafruit_PCF8575 _PCF1, _PCF2, _PCF3, _PCF4;
        Adafruit_PCF8575 _PCF[4] = { _PCF1, _PCF2, _PCF3, _PCF4 };
        int8_t _scl = -1;
        int8_t _sda = -1;
        int8_t _pwm = -1;
        int8_t _ws = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        LiteLED* _strip;

        void _print();
        void _sendToDisplay();
        void _clearDisplay();
};

/**
 * Display initialize
 * @param dispNum display number
 * @param scl display SCL pin number
 * @param sda display SDA pin number
 * @param pwm display PWM pin number
 * @param ws display Backlight pin number
 */
void PCF8575_S::init(uint8_t dispNum, int8_t scl, int8_t sda, int8_t pwm, int8_t ws) {
    _dispNum = dispNum;
    _scl = scl;
    _sda = sda;
    _pwm = pwm;
    _ws = ws;
    pinMode(pwm, OUTPUT);
    brightness(100, false);

    _setModel(config.display_model(dispNum));

    if(sda > 0 && scl > 0) {
        Wire1.begin(sda, scl, 400000);
        for(int i=0; i<4; i++) {
            _PCF[i].begin(0x20 + (dispNum * 4) + i, &Wire1);
        }
        vTaskDelay(100);

        _clearDisplay();
    }

    _strip = _dispNum == 0 ? &strip_1 : &strip_2;
    _strip->begin(ws, 8);
    _strip->clear(true);
}

/**
 * Set display brightness
 * @param intensity brightness
 * @param reduc brightness is reduced or not
 */
void PCF8575_S::brightness(uint8_t intensity, bool reduc) {
    _brightness = reduc ? round(intensity / 2) : intensity;
    uint16_t bright = map(_brightness, 0, 100, 200, 0);
    bright = constrain(bright, 0, 200);
    analogWrite(_pwm, bright);
}

/**
 * Displays the next display slot
 */
void PCF8575_S::refresh() {
    _slotSwitch();
    _segAnimations();
    _print();
}

/**
 * Display data on the display
 */
void PCF8575_S::_print() {
    bool updated = false;

    for(uint8_t i=0; i<8; i++) {
        if(_dispImg[i] >= DOT) _pixels[i] = font_pcf8575[WITH_DOT][_dispImg[i] - DOT];
        else _pixels[i] = font_pcf8575[SYMBOLS][_dispImg[i]];

        if(_pixelsPrev[i] != _pixels[i]) {
            _pixelsPrev[i] = _pixels[i];
            updated |= true;
        }
    }

    if(updated) _sendToDisplay();
}

/**
 * Send data to display
 */
void PCF8575_S::_sendToDisplay() {
    if(_sda > 0 && _scl > 0) {
        byte seg[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
        if(_power) {
            for(uint8_t i=0; i<8; i++) seg[i] = _pixels[i];
        }

        for(int i=0; i<4; i++) {
            uint16_t buf = (uint16_t)seg[i * 2];
            buf = buf << 8;
            buf += (uint16_t)seg[i * 2 + 1];
            if(numitronSemaphore != NULL) {
                if(xSemaphoreTake(numitronSemaphore, (TickType_t)100) == pdTRUE) {
                    _PCF[i].digitalWriteWord(buf);
                    xSemaphoreGive(numitronSemaphore);
                }
            }
        }
    }

    /* Backlight */
    uint8_t bright = (uint8_t)map(_brightness, 0, 100, 1, 180);
    bright = constrain(bright, 1, 180);
    _strip->brightness(bright, false);

    for(uint8_t i=0; i<8; i++) {
        unsigned int colors = strtol(&_dispColors[i][1], NULL, 16);
        rgb_t pixelColor = { 
            .r = uint8_t(colors >> 16), 
            .g = uint8_t(colors >> 8 & 0x00FF), 
            .b = uint8_t(colors & 0x00FF) 
        };
        _strip->setPixel(i, pixelColor, false);
    }

    _strip->show();
}

/**
 * Clear the display
 */
void PCF8575_S::_clearDisplay() {
    for(int i=0; i<8; i++) _pixels[i] = 0xFF;
    _print();
}