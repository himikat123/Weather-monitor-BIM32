class AgregateComfortData {
    public:
        float temp();
        float hum();
        float iaq();
        float co2();
};

float AgregateComfortData::temp() {
    float temp = 40400.0;
    switch(config.comfort.temp.source()) {
        case 1: temp = weather.get_currentTemp(); break; // temperature from weather forecast
        case 2: // temperature from wireless sensor
            if(wsensor.dataRelevance(config.comfort.temp.wsensNum()))
                temp = wsensor.get_temperature(config.comfort.temp.wsensNum(), config.comfort.temp.sens());
            break;
        case 3: // temperature from thingspeak
            if(thingspeak.dataRelevance()) 
                temp = thingspeak.get_field(config.comfort.temp.thing());
            break;
        case 4: temp = sensors.get_bme280_temp(); break; // temperature from BME280
        case 5: temp = sensors.get_bmp180_temp(); break; // temperature from BMP180
        case 6: temp = sensors.get_sht21_temp(); break; // temperature from SHT21
        case 7: temp = sensors.get_dht22_temp(); break; // temperature from DHT22
        case 8: temp = sensors.get_ds18b20_temp(); break; // temperature from DS18B20
        case 9: temp = sensors.get_bme680_temp(); break; // temperature from BME680
        default: ; break;
    }
    return temp;
}

float AgregateComfortData::hum() {
    float hum = 40400.0;
    switch(config.comfort.hum.source()) {
        case 1: hum = weather.get_currentHum(); break; // humidity from weather forecast
        case 2: // humidity from wireless sensor
            if(wsensor.dataRelevance(config.comfort.hum.wsensNum()))
                hum = wsensor.get_humidity(config.comfort.hum.wsensNum());
            break;
        case 3: // humidity from thingspeak
            if(thingspeak.dataRelevance())
                hum = thingspeak.get_field(config.comfort.hum.thing());
            break;
        case 4: hum = sensors.get_bme280_hum(); break; // humidity from BME280
        case 5: hum = sensors.get_sht21_hum(); break; // humidity from SHT21
        case 6: hum = sensors.get_dht22_hum(); break; // humidity from DHT22
        case 7: hum = sensors.get_bme680_hum(); break; // humidity from BME680
        default: ; break;
    }
    return hum;
}

float AgregateComfortData::iaq() {
    float iaq = 40400.0;
    if(config.comfort.iaq.source() == 1) {
        iaq = sensors.get_bme680_iaq();
    }
    return iaq;
}

float AgregateComfortData::co2() {
    int co2 = 40400.0;
    if(config.comfort.co2.source() == 1) {
        if(wsensor.dataRelevance(config.comfort.co2.wsensNum()))
            co2 = wsensor.get_co2(config.comfort.co2.wsensNum());
    }
    return co2;
}