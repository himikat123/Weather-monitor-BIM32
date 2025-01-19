class SoftI2C {
    protected:
        uint8_t _transmitting;
        uint8_t _error;
        uint8_t _sda;
        uint8_t _scl;

        bool _i2c_init(void);
        bool _i2c_start(uint8_t addr);
        bool _i2c_rep_start(uint8_t addr);
        void _i2c_stop(void);
        bool _i2c_write(uint8_t value);
        void _setSdaHigh(void);
        void _setSdaLow(void);
        void _setSclHigh(void);
        void _setSclLow(void);
        uint8_t _getSda(void);
  
    public:
        SoftI2C(uint8_t sda, uint8_t scl);
        void begin(void);
        void setPins(uint8_t sda, uint8_t scl);
        void beginTransmission(uint8_t address);
        uint8_t endTransmission(void);
        size_t write(uint8_t data);
};

SoftI2C::SoftI2C(uint8_t sda, uint8_t scl) {
    setPins(sda, scl);
}

void SoftI2C::begin(void) {
    _error = 0;
    _transmitting = false;
    _i2c_init();
}

void SoftI2C::setPins(uint8_t sda, uint8_t scl) {
    _sda = sda;
    _scl = scl;
}

void SoftI2C::beginTransmission(uint8_t address) {
    if(_transmitting) _error = (_i2c_rep_start((address << 1) | 0) ? 0 : 2);
    else _error = (_i2c_start((address << 1) | 0) ? 0 : 2);
    _transmitting = 1;
}

uint8_t SoftI2C::endTransmission() {
    uint8_t transError = _error;
    _i2c_stop();
    _transmitting = 0;
    _error = 0;
    return transError;
}

size_t SoftI2C::write(uint8_t data) {
    if(_i2c_write(data)) return 1;
    else {
        if(_error == 0) _error = 3;
        return 0;
    }
}

bool SoftI2C::_i2c_init(void) {
    pinMode(_sda, INPUT);
    digitalWrite(_sda, LOW);
    pinMode(_scl, INPUT);
    digitalWrite(_scl, LOW);
    delayMicroseconds(1);
    _setSclHigh();
    delayMicroseconds(1);
    _setSdaHigh();
    delayMicroseconds(10);
    if(_getSda() == 0) return false;
    return true;
}

bool SoftI2C::_i2c_start(uint8_t addr) {
    if(_getSda() == 0) return false;
    _setSdaLow();
    delayMicroseconds(1);
    _setSclLow();
    delayMicroseconds(1);
    return _i2c_write(addr);
}

bool SoftI2C::_i2c_rep_start(uint8_t addr) {
    _setSdaHigh();
    delayMicroseconds(1);
    _setSclHigh();
    delayMicroseconds(1);
    return _i2c_start(addr);
}

void SoftI2C::_i2c_stop(void) {
    _setSdaLow();
    delayMicroseconds(1);
    _setSclHigh();
    delayMicroseconds(1);
    _setSdaHigh();
    delayMicroseconds(1);
}

bool SoftI2C::_i2c_write(uint8_t value) {
    for(uint8_t curr = 0x80; curr != 0; curr >>= 1) {
        if(curr & value) _setSdaHigh(); 
        else _setSdaLow();
        _setSclHigh();
        delayMicroseconds(1);
        _setSclLow();
        delayMicroseconds(1);
    }
    _setSdaHigh();
    _setSclHigh();
    delayMicroseconds(1);
    uint8_t ack = _getSda();
    _setSclLow();
    delayMicroseconds(1);
    _setSdaLow();
    return ack == 0;
}

void SoftI2C::_setSdaLow(void) { 
    digitalWrite(_sda, LOW);
    pinMode(_sda, OUTPUT);
}

void SoftI2C::_setSdaHigh(void) {
    pinMode(_sda, INPUT);
}

void SoftI2C::_setSclLow(void) { 
	digitalWrite(_scl, LOW);
    pinMode(_scl, OUTPUT);
}

void SoftI2C::_setSclHigh(void) {
    pinMode(_scl, INPUT);
}

uint8_t SoftI2C::_getSda(void) {
    return digitalRead(_sda);
}