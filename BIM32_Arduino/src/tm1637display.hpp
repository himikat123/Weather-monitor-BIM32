#define DISP4 0
#define DISP6 1

class TM1637 : public SegmentDisplay {
    public:
        void init(uint8_t dispNum, int8_t clk, int8_t dat);
        void refresh();

    private:
        int8_t _clk = -1;
        int8_t _dat = -1;
        byte _pixels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        byte _pixelsPrev[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        void _print();
        void _sendToDisplay();
        void _clearDisplay();
        void _writeByte(byte data);
        void _start();
        void _stop(void);
};

/**
 * Display initialize
 * @param dispNum display number
 * @param clk display CLK pin number
 * @param dat display DAT pin number
 */
void TM1637::init(uint8_t dispNum, int8_t clk, int8_t dat) {
    _dispNum = dispNum;
    _clk = clk;
    _dat = dat;
    pinMode(clk, OUTPUT);
    pinMode(dat, OUTPUT);
    digitalWrite(clk, HIGH);
    digitalWrite(dat, HIGH);
    _setModel(config.display_model(dispNum));
    _clearDisplay();
}

/**
 * Displays the next display slot
 */
void TM1637::refresh() {
    _slotSwitch();
    _segAnimations();
    _print();
}

/**
 * Display data on the display
 */
void TM1637::_print() {
    bool updated = false;

    for(uint8_t i=0; i<6; i++) {
        if(_dispImg[i] >= 100) _pixels[i] = font_tm1637[1][_dispImg[i] - 100];
        else _pixels[i] = font_tm1637[0][_dispImg[i]];

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
void TM1637::_sendToDisplay() {
    uint8_t bright = map(_brightness, 0, 100, 0, 7);
    byte seg[6] = { 0, 0, 0, 0, 0, 0 };
    if(_power) {
        for(uint8_t i=0; i<6; i++) seg[i] = _pixels[config.display_order(_dispNum, i) - 1];
    }
    _start();
    _writeByte(0x40);
    _stop();
    _start();
    _writeByte(0xC0);
    for(uint8_t i=0; i<6; i++) {
        _writeByte(seg[i]);
    }
    _stop();
    _start();
    _writeByte(0x88 + bright);
    _stop();
}

/**
 * Clear the display
 */
void TM1637::_clearDisplay() {
    for(int i=0; i<6; i++) _pixels[i] = 0;
    _print();
}

/**
 * Send a byte to display
 */
void TM1637::_writeByte(byte data) {
    if(_clk > 0 && _dat > 0) {
        for(uint8_t i=0; i<8; i++) {
            //pinMode(_clk, OUTPUT);
            digitalWrite(_clk, LOW);
            delayMicroseconds(50);
            //if(data & 0x01) pinMode(_dat, INPUT);
            if(data & 0x01) digitalWrite(_dat, HIGH);
            //else pinMode(_dat, OUTPUT);
            else digitalWrite(_dat, LOW);
            data >>= 1;
            delayMicroseconds(50);
            //pinMode(_clk, INPUT);
            digitalWrite(_clk, HIGH);
            delayMicroseconds(50);
        }
        //pinMode(_clk, OUTPUT);
        digitalWrite(_clk, LOW);
        delayMicroseconds(50);
        //pinMode(_dat, INPUT);
        digitalWrite(_dat, HIGH);
        delayMicroseconds(50);
        //pinMode(_clk, INPUT);
        digitalWrite(_clk, HIGH);
        pinMode(_dat, INPUT);
        delayMicroseconds(50);
        uint8_t ack = digitalRead(_dat);
        if(ack == 0) {
            //pinMode(_dat, OUTPUT);
            digitalWrite(_dat, LOW);
        }
        delayMicroseconds(50);
        pinMode(_dat, OUTPUT);
        delayMicroseconds(50);
    }
}

/**
 * Sends start command to display
 */
void TM1637::_start() {
    if(_clk > 0 && _dat > 0) {
        //pinMode(_clk, INPUT);
        digitalWrite(_clk, HIGH);
        delayMicroseconds(50);
        //pinMode(_dat, INPUT);
        digitalWrite(_dat, HIGH);
        delayMicroseconds(50);
        //pinMode(_dat, OUTPUT);
        digitalWrite(_dat, LOW);
        delayMicroseconds(50);
        //pinMode(_clk, OUTPUT);
        digitalWrite(_clk, LOW);
        delayMicroseconds(50);
    }
}

/**
 * Sends stop command to display
 */
void TM1637::_stop(void) {
    if(_clk > 0 && _dat > 0) {
        //pinMode(_clk, OUTPUT);
        digitalWrite(_clk, LOW);
        delayMicroseconds(50);
        //pinMode(_dat, OUTPUT);
        digitalWrite(_dat, LOW);
        delayMicroseconds(50);
        //pinMode(_clk, INPUT);
        digitalWrite(_clk, HIGH);
        delayMicroseconds(50);
        //pinMode(_dat, INPUT);
        digitalWrite(_dat, HIGH);
        delayMicroseconds(50);
    }
}