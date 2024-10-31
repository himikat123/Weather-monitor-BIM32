class Validate {
    public:
        bool temp(float t);
        bool hum(float h);
        bool pres(float p);
        bool wind(float w);
        bool light(float l);
        bool volt(float v);
        bool iaq(float i);
        bool batVolt(float v);
        bool batLvl(int l);
        bool batPercent(int p);
        bool thingVolt(float v);
        bool hVolt(float v);
        bool current(float c);
        bool power(float p);
        bool energy(float e);
        bool frequence(float f);
        bool co2(float c);
};

/**
 * Validate if temperature is within the normal range
 */
bool Validate::temp(float t) {
    return (t >= -55.0 and t <= 100.0);
}

/**
 * Validate if humidity is within the normal range
 */
bool Validate::hum(float h) {
    return (h >= 0.0 and h <= 100.0);
}

/**
 * Validate if pressure is within the normal range
 */
bool Validate::pres(float p) {
    return (p >= 800.0 and p <= 1200.0);
}

/**
 * Validate if wind speed is within the normal range
 */
bool Validate::wind(float w) {
    return (w >= 0.0 and w <= 100.0);
}

/**
 * Validate if ambient light is within the normal range
 */
bool Validate::light(float l) {
    return (l >= 0.0 and l <= 120000.0);
}

/**
 * Validate if analog voltage is within the normal range
 */
bool Validate::volt(float v) {
    return (v >= 0.0 and v <= 3.3);
}

/**
 * Validate if IAQ is within the normal range
 */
bool Validate::iaq(float i) {
    return (i >= 0.0 and i <= 500.0);
}

/**
 * Validate if battery voltage is within the normal range
 */
bool Validate::batVolt(float v) {
    return (v >= 0.0 and v <= 12.0);
}

/**
 * Validate if battery level is within the normal range
 */
bool Validate::batLvl(int l) {
    return (l >= 1 and l <= 4);
}

/**
 * Validate if battery percentage is within the normal range
 */
bool Validate::batPercent(int p) {
    return (p >= 0 and p <= 100);
}

/**
 * Validate if voltage from thingspeak is within the normal range
 */
bool Validate::thingVolt(float v) {
    return (v >= 0.0 and v <= 260.0);
}

/**
 * Validate if PZEM-004t voltage is within the normal range
 */
bool Validate::hVolt(float v) {
    return (v >= 80.0 and v <= 260.0);
}

/**
 * Validate if PZEM-004t current is within the normal range
 */
bool Validate::current(float c) {
    return (c >= 0.0 and c <= 100.0);
}

/**
 * Validate if PZEM-004t power is within the normal range
 */
bool Validate::power(float p) {
    return (p >= 0.0 and p <= 23000.0);
}

/**
 * Validate if PZEM-004t energy is within the normal range
 */
bool Validate::energy(float e) {
    return (e >= 0.0 and e <= 10000.0);
}

/**
 * Validate if PZEM-004t frequency is within the normal range
 */
bool Validate::frequence(float f) {
    return (f >= 45.0 and f <= 65.0);
}

/**
 * Validate if Senseair S8 CO2 is within the normal range
 */
bool Validate::co2(float c) {
    return (c >= 400.0 and c <= 2000.0);
}