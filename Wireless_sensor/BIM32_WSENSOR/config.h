/**
 * Temperature, humidity, pressure sensor
 *  
 * Uncomment only one of this sensors if you need one of them
 */
// #define USE_BME280
// #define USE_SHT21

/**
 * Temperature sensor(s)
 * 
 * Uncomment as many sensors as you need them
 */
// #define USE_DS18B20_1
// #define USE_DS18B20_2
// #define USE_DS18B20_3
// #define USE_DS18B20_4

/**
 * Set DS18B20 resolution
 * 9  -> accuracy: 0.5°C,    measurement time: 93ms
 * 10 -> accuracy: 0.25°C,   measurement time: 187ms
 * 11 -> accuracy: 0.125°C,  measurement time: 375ms
 * 12 -> accuracy: 0.0625°C, measurement time: 750ms
 */
// #define DS18B20_RESOLUTION 9
// #define DS18B20_RESOLUTION 10
 #define DS18B20_RESOLUTION 11
// #define DS18B20_RESOLUTION 12

/**
 * Ambient light sensor
 * 
 * Uncomment if you need it
 */
// #define USE_MAX44009


/**
 * Energy monitor PZEM-004T
 * 
 * Uncomment if you need it
 */
// #define USE_PZEM_004T


/**
 * CO2 sensor SENSEAIR S8
 * 
 * Uncomment if you need it
 */
// #define USE_S8


/**
 * The number of this wireless sensor. Change this if you 
 * are using multiple wireless sensors at the same time
 * 0 or 1
 */
const int SENSOR_NUMBER = 0;
