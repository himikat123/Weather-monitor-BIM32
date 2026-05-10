class AgregateComfortData {
    private:
        static constexpr int WEATHER_TEMP = 1;
        static constexpr int WSENSOR_TEMP = 2;
        static constexpr int THING_TEMP   = 3;
        static constexpr int BME280_TEMP  = 4;
        static constexpr int BMP180_TEMP  = 5;
        static constexpr int SHT21_TEMP   = 6;
        static constexpr int DHT22_TEMP   = 7;
        static constexpr int DS18B20_TEMP = 8;
        static constexpr int BME680_TEMP  = 9;

        static constexpr int WEATHER_HUM  = 1;
        static constexpr int WSENSOR_HUM  = 2;
        static constexpr int THING_HUM    = 3;
        static constexpr int BME280_HUM   = 4;
        static constexpr int SHT21_HUM    = 5;
        static constexpr int DHT22_HUM    = 6;
        static constexpr int BME680_HUM   = 7;

        static constexpr int BME680_IAQ   = 1;
        static constexpr int WSENSOR_CO2  = 1;

    public:
        float temp();
        float hum();
        float iaq();
        float co2();
};

float AgregateComfortData::temp() {
    float temp = UNDEFINED_FLOAT;
    switch(config.comfort.temp.source()) {
        case WEATHER_TEMP: temp = weather.get_currentTemp(); break; // temperature from weather forecast
        case WSENSOR_TEMP: // temperature from wireless sensor
            if(wsensor.dataRelevance(config.comfort.temp.wsensNum()))
                temp = wsensor.get_temperature(config.comfort.temp.wsensNum(), config.comfort.temp.sens());
            break;
        case THING_TEMP: // temperature from thingspeak
            if(thingspeak.dataRelevance()) 
                temp = thingspeak.get_field(config.comfort.temp.thing());
            break;
        case BME280_TEMP: temp = sensors.get_bme280_temp(); break; // temperature from BME280
        case BMP180_TEMP: temp = sensors.get_bmp180_temp(); break; // temperature from BMP180
        case SHT21_TEMP: temp = sensors.get_sht21_temp(); break; // temperature from SHT21
        case DHT22_TEMP: temp = sensors.get_dht22_temp(); break; // temperature from DHT22
        case DS18B20_TEMP: temp = sensors.get_ds18b20_temp(); break; // temperature from DS18B20
        case BME680_TEMP: temp = sensors.get_bme680_temp(); break; // temperature from BME680
        default: ; break;
    }
    return temp;
}

float AgregateComfortData::hum() {
    float hum = UNDEFINED_FLOAT;
    switch(config.comfort.hum.source()) {
        case WEATHER_HUM: hum = weather.get_currentHum(); break; // humidity from weather forecast
        case WSENSOR_HUM: // humidity from wireless sensor
            if(wsensor.dataRelevance(config.comfort.hum.wsensNum()))
                hum = wsensor.get_humidity(config.comfort.hum.wsensNum());
            break;
        case THING_HUM: // humidity from thingspeak
            if(thingspeak.dataRelevance())
                hum = thingspeak.get_field(config.comfort.hum.thing());
            break;
        case BME280_HUM: hum = sensors.get_bme280_hum(); break; // humidity from BME280
        case SHT21_HUM: hum = sensors.get_sht21_hum(); break; // humidity from SHT21
        case DHT22_HUM: hum = sensors.get_dht22_hum(); break; // humidity from DHT22
        case BME680_HUM: hum = sensors.get_bme680_hum(); break; // humidity from BME680
        default: ; break;
    }
    return hum;
}

float AgregateComfortData::iaq() {
    float iaq = UNDEFINED_FLOAT;
    if(config.comfort.iaq.source() == BME680_IAQ) {
        iaq = sensors.get_bme680_iaq();
    }
    return iaq;
}

float AgregateComfortData::co2() {
    int co2 = UNDEFINED_FLOAT;
    if(config.comfort.co2.source() == WSENSOR_CO2) {
        if(wsensor.dataRelevance(config.comfort.co2.wsensNum()))
            co2 = wsensor.get_co2(config.comfort.co2.wsensNum());
    }
    return co2;
}