#define DISP4    0
#define DISP6    1
#define DOT    100
#define SYMBOLS  0
#define WITH_DOT 1

class PCF8575_S : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t scl, int8_t sda, int8_t pwm, int8_t ws);
        void brightness(uint8_t intensity, bool reduc);
        void displayToggle();
        void displayOn();
        void displayOff();
        void refresh();

    private:
        SoftI2C* _wire = nullptr;
        LiteLED* _strip = nullptr;
        int8_t _pwm = -1;
        int8_t _ws = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _prevPixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};

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
    _pwm = pwm;
    _ws = ws;
    pinMode(pwm, OUTPUT);
    brightness(100, false);

    _setModel(config.display_model(dispNum));

    _wire = dispNum == 0 ? &Wire_1 : &Wire_2;
    _clearDisplay();

    _strip = dispNum == 0 ? &strip_1 : &strip_2;
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
    uint8_t bright = (uint8_t)map(_brightness, 1, 100, 180, 1);
    bright = constrain(bright, 1, 180);
    analogWrite(_pwm, bright);

    bright = (uint8_t)map(_brightness, 1, 100, 1, 60);
    bright = constrain(bright, 1, 60);
    _strip->brightness(bright, true);
}

/**
 * Toggle display (on/off)
 */
void PCF8575_S::displayToggle() {
    _power = !_power;
    _sendToDisplay();
}

/**
 * Turn display on
 */
void PCF8575_S::displayOn() {
    _power = true;
    _sendToDisplay();
}

/*
 * Turn display off
 */
void PCF8575_S::displayOff() {
    _power = false;
    _sendToDisplay();
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

        if(_prevPixels[i] != _pixels[i]) {
            _prevPixels[i] = _pixels[i];
            updated |= true;
        }
    }

    if(updated || global.colorChanged) {
        _sendToDisplay();
        global.colorChanged = false;
    }
}

/**
 * Send data to display
 */
void PCF8575_S::_sendToDisplay() {
    byte seg[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    if(_power) {
        for(uint8_t i=0; i<8; i++) seg[i] = _pixels[i];
    }

    for(int i = 0; i < (_dispLength / 2); i++) {
        _wire->beginTransmission(0x20 + i);
        _wire->write(seg[i * 2 + 1]);
        _wire->write(seg[i * 2]);
        _wire->endTransmission();
    }

    /* Backlight */
    rgb_t black = { .r = 0, .g = 0, .b = 0 };

    for(uint8_t i=0; i<8; i++) {
        unsigned int colors = strtol(&_dispColors[i][1], NULL, 16);
        rgb_t pixelColor = { 
            .r = uint8_t(colors >> 16), 
            .g = uint8_t(colors >> 8 & 0x00FF), 
            .b = uint8_t(colors & 0x00FF) 
        };
        _strip->setPixel(i, _power ? pixelColor : black, false);
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