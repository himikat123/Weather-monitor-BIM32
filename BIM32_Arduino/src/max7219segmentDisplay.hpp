#include "./max7219driver.hpp"

#define DISP4 0
#define DISP6 1
#define DISP8 2

class MAX7219_S : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t clk, int8_t dat, int8_t load);
        void refresh();
        void brightness(uint8_t intensity, bool reduc);
        void displayToggle();
        void displayOn();
        void displayOff();

    private:
        MAX7219_Driver max7219;
        int8_t _clk = -1;
        int8_t _dat = -1;
        int8_t _load = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        void _print();
        void _sendToDisplay();
        void _clearDisplay();
};

/**
 * Display initialize
 * @param dispNum display number
 * @param clk display CLK pin number
 * @param dat display DAT pin number
 * @param load display LOAD pin number
 */
void MAX7219_S::init(uint8_t dispNum, int8_t clk, int8_t dat, int8_t load) {
    _dispNum = dispNum;
    _clk = clk;
    _dat = dat;
    _load = load;
    pinMode(dat, OUTPUT);
    pinMode(clk, OUTPUT);
    pinMode(load, OUTPUT);
    digitalWrite(load, HIGH);
    max7219.init(clk, dat, load, 0);
    _setModel(config.display_model(dispNum));
}

/**
 * Displays the next display slot
 */
void MAX7219_S::refresh() {
    _slotSwitch();
    _segAnimations();
    _print();
}

/**
 * Display data on the display
 */
void MAX7219_S::_print() {
    bool updated = false;

    for(uint8_t i=0; i<8; i++) {
        if(_dispImg[i] >= 100) _pixels[i] = font_max7219[1][_dispImg[i] - 100];
        else _pixels[i] = font_max7219[0][_dispImg[i]];

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
void MAX7219_S::_sendToDisplay() {
    byte seg[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    if(_power) {
        for(uint8_t i=0; i<8; i++) seg[i] = _pixels[config.display_order(_dispNum, i) - 1];
    }
    max7219.print(seg);
}

/**
 * Clear the display
 */
void MAX7219_S::_clearDisplay() {
    for(int i=0; i<8; i++) _pixels[i] = 0;
    _print();
}

/**
 * Toggle display on/off
 */
void MAX7219_S::displayToggle() {
    _power = !_power;
    max7219.power(_power);
}

/**
 * Turn on the display
 */
void MAX7219_S::displayOn() {
    _power = true;
    max7219.power(true);
}

/*
 * Turn off the display
 */
void MAX7219_S::displayOff() {
    _power = false;
    max7219.power(false);
}

/**
 * Set display brightness
 * @param brightness intensity
 * @param reduc brightness is reduced or not
 */
void MAX7219_S::brightness(uint8_t intensity, bool reduc) {
    _brightness = reduc ? round(intensity / 2) : intensity;
    uint8_t bright = map(_brightness, 0, 100, 0, 15);
    bright = constrain(bright, 0, 15);
    max7219.setIntensity(bright);
}