#define DISP4 0
#define DISP6 1
#define DISP8 2

class MAX7219_Driver {
    public:
        void init(int8_t clk, int8_t dat, int8_t load, uint8_t chip);
        void power(bool state);
        void setIntensity(uint8_t intensity);
        void print(byte* img);
        void clearDisplay();

    private:
        int8_t _clk = -1;
        int8_t _dat = -1;
        int8_t _load = -1;
        uint8_t _chip = 0;

        void _spiTransfer(int addr, volatile byte opcode, volatile byte data);
};

/**
 * MAX7219 initialize
 * @param clk display CLK pin number
 * @param dat display DAT pin number
 * @param load display LOAD pin number
 * @param chip chip number in series
 */
void MAX7219_Driver::init(int8_t clk, int8_t dat, int8_t load, uint8_t chip) {
    _clk = clk;
    _dat = dat;
    _load = load;
    _chip = chip;
    _spiTransfer(0, 15, 0);
    _spiTransfer(0, 11, 7);
    _spiTransfer(0, 9, 0);
    clearDisplay();
    _spiTransfer(0, 12, 1);
    _spiTransfer(0, 10, 7);
}

/**
 * Turn display on/off
 */
void MAX7219_Driver::power(bool state) {
    _spiTransfer(0, 12, state ? 1 : 0);
}

/**
 * Set display brightness
 * @param brightness intensity
 */
void MAX7219_Driver::setIntensity(uint8_t intensity) {
    if(intensity > 15) intensity = 15;
    _spiTransfer(0, 10, intensity);
}

/**
 * Send data to display
 */
void MAX7219_Driver::print(byte* img) {
    for(uint8_t i=0; i<8; i++) {
        _spiTransfer(0, i + 1, img[7 - i]);
    }
}

/**
 * Clear display
 */
void MAX7219_Driver::clearDisplay() {
    for(int i=1; i<9; i++) {
        _spiTransfer(0, i, 0);
    }
}

/**
 * Send a byte to display
 */
void MAX7219_Driver::_spiTransfer(int addr, volatile byte opcode, volatile byte data) {
    if(_clk > 0 && _dat > 0 && _load > 0) {
        int offset = addr * 2;
        byte spidata[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        spidata[offset + 1] = opcode;
        spidata[offset] = data;
        digitalWrite(_load, LOW);
        for(int i=16; i>0; i--) shiftOut(_dat, _clk, MSBFIRST, spidata[i - 1]);
        digitalWrite(_load, HIGH);
    }
}
