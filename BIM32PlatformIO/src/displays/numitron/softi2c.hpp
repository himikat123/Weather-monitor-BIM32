#pragma once

class SoftI2C {
    protected:
        uint8_t _transmitting = 0;
        uint8_t _error = 0;
        uint8_t _sda = 0;
        uint8_t _scl = 0;

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