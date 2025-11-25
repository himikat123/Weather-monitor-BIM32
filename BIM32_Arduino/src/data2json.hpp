class Data2json {
    private:
        String _filelist;
        void _listAllFilesInDir(String dirname);

    public:
        String firmware();
        String system();
        String network();
        String bme280();
        String bmp180();
        String sht21();
        String dht22();
        String ds18b20();
        String max44009();
        String bh1750();
        String bme680();
        String thingspeak();
        String weather();
        String wsensor();
        String filesystem();
};

String Data2json::firmware() {
    JsonDocument json;

    json["fw"] = FW;
    #if defined(BIM32_CYD)
        json["cyd"] = 1;
    #endif

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::system() {
    JsonDocument json;

    json["runtime"] = round(millis() / 1000);
    json["heap"] = ESP.getFreeHeap();
    json["time"] = now();
    json["esp32"]["temp"] = sensors.get_esp32_temp(RAW);
    json["analog"]["volt"] = sensors.get_analog_voltage(RAW);

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::network() {
    JsonDocument json;
    
    for(unsigned int i=0; i<global.nets; i++) {
        json["ssids"][i][0] = global.ssids[i];
        json["ssids"][i][1] = global.rssis[i];  
    }
    json["network"]["ssid"] = global.apMode ? config.accessPoint_ssid() : WiFi.SSID();
    json["network"]["ch"] = WiFi.channel();
    json["network"]["sig"] = WiFi.RSSI();
    json["network"]["mac"] = WiFi.macAddress();
    json["network"]["ip"] = WiFi.localIP().toString();
    json["network"]["mask"] = WiFi.subnetMask().toString();
    json["network"]["gw"] = WiFi.gatewayIP().toString();
    json["network"]["dns1"] = WiFi.dnsIP().toString();
    json["network"]["dns2"] = WiFi.dnsIP().toString();
    
    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::bme280() {
    JsonDocument json;

    json["bme280"]["temp"] = sensors.get_bme280_temp(RAW);
    json["bme280"]["hum"] = sensors.get_bme280_hum(RAW);
    json["bme280"]["pres"] = sensors.get_bme280_pres(RAW);

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::bmp180() {
    JsonDocument json;

    json["bmp180"]["temp"] = sensors.get_bmp180_temp(RAW);
    json["bmp180"]["pres"] = sensors.get_bmp180_pres(RAW);

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::sht21() {
    JsonDocument json;

    json["sht21"]["temp"] = sensors.get_sht21_temp(RAW);
    json["sht21"]["hum"] = sensors.get_sht21_hum(RAW);

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::dht22() {
    JsonDocument json;

    json["dht22"]["temp"] = sensors.get_dht22_temp(RAW);
    json["dht22"]["hum"] = sensors.get_dht22_hum(RAW);

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::ds18b20() {
    String data = "{\"ds18b20\": {\"temp\": " + String(sensors.get_ds18b20_temp(RAW)) + "}}";
    return data;
}

String Data2json::max44009() {
    String data = "{\"max44009\": {\"light\": " + String(sensors.get_max44009_light(RAW)) + "}}";
    return data;
}

String Data2json::bh1750() {
    String data = "{\"bh1750\": {\"light\": " + String(sensors.get_bh1750_light(RAW)) + "}}";
    return data;
}

String Data2json::bme680() {
    JsonDocument json;

    json["bme680"]["temp"] = sensors.get_bme680_temp(RAW);
    json["bme680"]["hum"] = sensors.get_bme680_hum(RAW);
    json["bme680"]["pres"] = sensors.get_bme680_pres(RAW);
    json["bme680"]["iaq"] = sensors.get_bme680_iaq(RAW);
    json["bme680"]["iaqAccr"] = sensors.get_bme680_iaq_accuracy();

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::thingspeak() {
    JsonDocument json;

    json["thing"]["time"] = thingspeak.get_updated();
    for(unsigned int i=0; i<THNG_FIELDS; i++) {
        json["thing"]["data"][i] = thingspeak.get_field(i);
    }

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::weather() {
    JsonDocument json;

    json["weather"]["temp"] = weather.get_currentTemp(RAW);
    json["weather"]["hum"] = weather.get_currentHum(RAW);
    json["weather"]["pres"] = weather.get_currentPres(RAW);
    json["weather"]["wind"]["speed"] = weather.get_currentWindSpeed();
    json["weather"]["wind"]["dir"] = weather.get_currentWindDir();
    json["weather"]["descript"] = weather.get_description();
    json["weather"]["time"] = weather.get_currentUpdated();
    json["weather"]["icon"] = weather.get_currentIcon();
    json["weather"]["isDay"] = weather.get_isDay();
    for(uint8_t i=0; i<4; i++) {
        json["weather"]["daily"]["tMax"][i] = weather.get_dailyDayTemp(i);
        json["weather"]["daily"]["tMin"][i] = weather.get_dailyNightTemp(i);
        json["weather"]["daily"]["wind"][i] = weather.get_dailyWindSpeed(i);
        json["weather"]["daily"]["icon"][i] = weather.get_dailyIcon(i);
    }

    String data = "";
    serializeJson(json, data);
    return data;
}

String Data2json::wsensor() {
    JsonDocument json;

    for(unsigned int i=0; i<WSENSORS; i++) {
        json["wsensor"]["time"][i] = wsensor.get_updated(i);
        for(unsigned int n=0; n<5; n++) {
            json["wsensor"]["temp"]["data"][n][i] = wsensor.get_temperature(i, n, RAW);
            json["wsensor"]["temp"]["name"][n][i] = wsensor.get_sensorType(i, n);
        }
        json["wsensor"]["hum"]["data"][i] = wsensor.get_humidity(i, RAW);
        json["wsensor"]["hum"]["name"][i] = wsensor.get_sensorType(i, 0);
        json["wsensor"]["pres"]["data"][i] = wsensor.get_pressure(i, RAW);
        json["wsensor"]["pres"]["name"][i] = wsensor.get_sensorType(i, 0);
        json["wsensor"]["light"]["data"][i] = wsensor.get_light(i, RAW);
        json["wsensor"]["light"]["name"][i] = wsensor.get_lightType(i);
        json["wsensor"]["voltage"]["data"][i] = wsensor.get_voltage(i, RAW);
        json["wsensor"]["voltage"]["name"][i] = wsensor.get_energyType();
        json["wsensor"]["current"]["data"][i] = wsensor.get_current(i, RAW);
        json["wsensor"]["current"]["name"][i] = wsensor.get_energyType();
        json["wsensor"]["power"]["data"][i] = wsensor.get_power(i, RAW);
        json["wsensor"]["power"]["name"][i] = wsensor.get_energyType();
        json["wsensor"]["energy"]["data"][i] = wsensor.get_energy(i, RAW);
        json["wsensor"]["energy"]["name"][i] = wsensor.get_energyType();
        json["wsensor"]["freq"]["data"][i] = wsensor.get_frequency(i, RAW);
        json["wsensor"]["freq"]["name"][i] = wsensor.get_energyType();
        json["wsensor"]["co2"]["data"][i] = wsensor.get_co2(i, RAW);
        json["wsensor"]["co2"]["name"][i] = wsensor.get_co2Type();
        json["wsensor"]["bat"][i] = wsensor.get_batteryAdc(i);
    }

    String data = "";
    serializeJson(json, data);
    return data;
}

void _listAllFilesInDir(String dirname) {
    File root = LittleFS.open(dirname);
    if(!root) return;
    if(!root.isDirectory()) return;

    File file = root.openNextFile();
    while(file) {
        if(file.isDirectory()) _listAllFilesInDir(String(file.path()) + "/");
        else {
            String filename = file.name();
            if(filename != "user.us") {
                _filelist += dirname;
                _filelist += filename;
                _filelist += ":";
                _filelist += String(file.size());
                _filelist += ",";
            }
        }
        file = root.openNextFile();
    }
}

String Data2json::filesystem() {
    JsonDocument json;

    _filelist = String();
    _listAllFilesInDir("/");
    size_t fsTotal = LittleFS.totalBytes();
    size_t fsUsed = fsTotal - LittleFS.usedBytes();

    json["fs"]["list"] = _filelist;
    json["fs"]["total"] = fsTotal;
    json["fs"]["free"] = fsUsed;

    String data = "";
    serializeJson(json, data);
    return data;
}