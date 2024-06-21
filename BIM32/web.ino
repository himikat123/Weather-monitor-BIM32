RTC_DATA_ATTR double code = esp_random();
size_t content_len;
String web_filelist = "";

/**
 * Get current date and time as a string
 */
String web_timeString(unsigned int tm) {
    char buf[20];
    sprintf(buf, "%02d:%02d:%02d %02d.%02d.%d", hour(tm), minute(tm), second(tm), day(tm), month(tm), year(tm));
    return String(buf);
}

void web_listAllFilesInDir(String dirname) {
    File root = LittleFS.open(dirname);
    if(!root) return;
    if(!root.isDirectory()) return;

    File file = root.openNextFile();
    while(file) {
        if(file.isDirectory()) web_listAllFilesInDir(String(file.path()) + "/");
        else {
            String filename = file.name();
            if(filename != "user.us") {
                web_filelist += dirname;
                web_filelist += filename;
                web_filelist += ":";
                web_filelist += String(file.size());
                web_filelist += ",";
            }
        }
        file = root.openNextFile();
    }
}

/**
 * Prepare sensors data
 */
String web_sensorsPrepare(bool logged) {
    JsonDocument json;
    json["state"] = logged ? "OK" : "LOGIN";
    json["fw"] = FW;
    json["runtime"] = round(millis() / 1000);
    json["heap"] = ESP.getFreeHeap();
    json["time"] = now();
    int wd = weekday();
    json["wd"][0] = lang.weekdayShortName(wd);
    if(++wd > 7) wd = 1;
    json["wd"][1] = lang.weekdayShortName(wd);
    if(++wd > 7) wd = 1;
    json["wd"][2] = lang.weekdayShortName(wd);
    if(++wd > 7) wd = 1;
    json["wd"][3] = lang.weekdayShortName(wd);
    json["units"]["v"] = lang.v();
    json["units"]["mm"] = lang.mm();
    json["units"]["ms"] = lang.ms();

    json["network"]["ssid"] = global.apMode ? config.accessPoint_ssid() : WiFi.SSID();
    json["network"]["ch"] = WiFi.channel();
    json["network"]["sig"] = WiFi.RSSI();
    json["network"]["mac"] = WiFi.macAddress();
    json["network"]["ip"] = WiFi.localIP().toString();
    json["network"]["mask"] = WiFi.subnetMask().toString();
    json["network"]["gw"] = WiFi.gatewayIP().toString();
    json["network"]["dns1"] = WiFi.dnsIP().toString();
    json["network"]["dns2"] = WiFi.dnsIP().toString();

    json["bme280"]["temp"] = sensors.get_bme280_temp(0);
    json["bme280"]["hum"] = sensors.get_bme280_hum(0);
    json["bme280"]["pres"] = sensors.get_bme280_pres(0);
    json["bmp180"]["temp"] = sensors.get_bmp180_temp(0);
    json["bmp180"]["pres"] = sensors.get_bmp180_pres(0);
    json["sht21"]["temp"] = sensors.get_sht21_temp(0);
    json["sht21"]["hum"] = sensors.get_sht21_hum(0);
    json["dht22"]["temp"] = sensors.get_dht22_temp(0);
    json["dht22"]["hum"] = sensors.get_dht22_hum(0);
    json["esp32"]["temp"] = sensors.get_esp32_temp(0);
    json["ds18b20"]["temp"] = sensors.get_ds18b20_temp(0);
    json["max44009"]["light"] = sensors.get_max44009_light(0);
    json["bh1750"]["light"] = sensors.get_bh1750_light(0);
    json["analog"]["volt"] = sensors.get_analog_voltage(0);
    json["bme680"]["temp"] = sensors.get_bme680_temp(0);
    json["bme680"]["hum"] = sensors.get_bme680_hum(0);
    json["bme680"]["pres"] = sensors.get_bme680_pres(0);
    json["bme680"]["iaq"] = sensors.get_bme680_iaq(0);
    json["bme680"]["iaqAccr"] = sensors.get_bme680_iaq_accuracy();

    json["thing"]["time"] = thingspeak.get_updated();
    for(unsigned int i=0; i<THNG_FIELDS; i++) {
        json["thing"]["data"][i] = thingspeak.get_field(i);
    }

    json["weather"]["temp"] = weather.get_currentTemp();
    json["weather"]["hum"] = weather.get_currentHum();
    json["weather"]["pres"] = weather.get_currentPres();
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

    for(unsigned int i=0; i<global.nets; i++) {
        json["ssids"][i][0] = global.ssids[i];
        json["ssids"][i][1] = global.rssis[i];  
    }

    for(unsigned int i=0; i<WSENSORS; i++) {
        json["wsensor"]["time"][i] = wsensor.get_updated(i);
        for(unsigned int n=0; n<5; n++) {
            json["wsensor"]["temp"]["data"][n][i] = wsensor.get_temperature(i, n, 0);
            json["wsensor"]["temp"]["name"][n][i] = wsensor.get_sensorType(i, n);
        }
        json["wsensor"]["hum"]["data"][i] = wsensor.get_humidity(i, 0);
        json["wsensor"]["hum"]["name"][i] = wsensor.get_sensorType(i, 0);
        json["wsensor"]["pres"]["data"][i] = wsensor.get_pressure(i, 0);
        json["wsensor"]["pres"]["name"][i] = wsensor.get_sensorType(i, 0);
        json["wsensor"]["light"]["data"][i] = wsensor.get_light(i, 0);
        json["wsensor"]["light"]["name"][i] = wsensor.get_lightType(i);
        json["wsensor"]["voltage"]["data"][i] = wsensor.get_voltage(i, 0);
        json["wsensor"]["voltage"]["name"][i] = wsensor.get_energyType(i);
        json["wsensor"]["current"]["data"][i] = wsensor.get_current(i, 0);
        json["wsensor"]["current"]["name"][i] = wsensor.get_energyType(i);
        json["wsensor"]["power"]["data"][i] = wsensor.get_power(i, 0);
        json["wsensor"]["power"]["name"][i] = wsensor.get_energyType(i);
        json["wsensor"]["energy"]["data"][i] = wsensor.get_energy(i, 0);
        json["wsensor"]["energy"]["name"][i] = wsensor.get_energyType(i);
        json["wsensor"]["freq"]["data"][i] = wsensor.get_frequency(i, 0);
        json["wsensor"]["freq"]["name"][i] = wsensor.get_energyType(i);
        json["wsensor"]["co2"]["data"][i] = wsensor.get_co2(i, 0);
        json["wsensor"]["co2"]["name"][i] = wsensor.get_co2Type(i);
        json["wsensor"]["bat"][i] = wsensor.get_batteryAdc(i);
    }

    web_listAllFilesInDir("/");
    json["fs"]["list"] = web_filelist;
    web_filelist = String();
    json["fs"]["total"] = LittleFS.totalBytes();
    json["fs"]["free"] = LittleFS.totalBytes() - LittleFS.usedBytes();
    
    String data = "";
    serializeJson(json, data);
    return data;
}

/**
 * Returns the value of the cookie
 */
String web_getCookieValue(String searchString, String stringName) {
    stringName += "=";
    int length = stringName.length();
    int pozStart = searchString.indexOf(stringName) + length;
    int pozStop = searchString.indexOf(";", pozStart);
    String cookieValue = searchString.substring(pozStart, pozStop); 
    return cookieValue;
}

/**
 * Checks if the user is logged in
 */
bool web_isLogged(AsyncWebServerRequest *request) {
    if(!config.account_required()) return true;

    if(request->hasHeader("Cookie")) {
        AsyncWebHeader* h = request->getHeader("Cookie");
        String auth = web_getCookieValue(h->value().c_str(), "code");
        if(auth == String(code)) return true;
        else return false;
    }
    else return false;
}

/**
 * Checks if the user is logged in and sends the requested file, otherwise sends the login page
 */
bool web_fileRead(AsyncWebServerRequest *request) {
    String path = request->url();
    if(!path.endsWith(".ico") && !path.endsWith(".png") && !path.endsWith(".jpg") && !path.endsWith(".vlw")) {
        if(path.endsWith(".json")) {
            if(!web_isLogged(request)) {
                request->send(200, "text/plain", "{\"lang\": \"" + config.lang() + "\", \"state\": \"LOGIN\"}");
                return true;
            }
        }
        else path = "/index.html";
    }
    String pathWithGz = path + ".gz";
    if(LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
        if(LittleFS.exists(pathWithGz)) path += ".gz";
        AsyncWebServerResponse *response = request->beginResponse(LittleFS, path);
        if(request->hasArg("download")) response->addHeader("Content-Type", "application/octet-stream");
        else if(path.endsWith(".json")) response->addHeader("Content-Type", "application/json");
        else if(path.endsWith(".ico")) response->addHeader("Content-Type", "image/x-icon");
        else if(path.endsWith(".png")) response->addHeader("Content-Type", "image/png");
        else if(path.endsWith(".jpg")) response->addHeader("Content-Type", "image/jpg");
        else if(path.endsWith(".vlw")) response->addHeader("Content-Type", "application/x-binary");
        else if(path.endsWith(".gz")) {
            response->addHeader("Content-Encoding", "gzip");
            response->addHeader("Content-Type", "text/html");
        }
        else response->addHeader("Content-Type", "text/plain");
        request->send(response);
        return true;
    }
    return false;
}

/**
 * Login
 * Username and password validation 
 */
void web_login(AsyncWebServerRequest *request) {
    String login = "";
    String pass = "";
    if(request->hasArg("name")) login = request->arg("name");
    if(request->hasArg("pass")) pass = request->arg("pass");
    bool loged = false;
    if(login == config.account_name() and pass == config.account_pass()) {
        loged = true;
        code = esp_random();
    }
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", loged ? "OK" : "error");
    response->addHeader("Cache-Control", "no-cache");
    String cookie = "code=";
    cookie += loged ? String(code) : "0";
    cookie += "; Max-Age=7200; Path=/";
    response->addHeader("Set-Cookie", cookie);
    request->send(response);
}

/**
 * Saves config data to config file
 */
void web_save(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("config")) {
            if(request->arg("config")) 
                request->send(200, "text/plain", config.save(request->arg("config")) ? "SAVE ERROR" : "OK");
            else request->send(200, "text/plain", "CONFIG ARGUMENT EMPTY");
        }
        else request->send(200, "text/plain", "NO CONFIG ARGUMENT");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Sends sensors data via AJAX
 */
void web_sens(AsyncWebServerRequest *request) {
    request->send(200, "text/json", web_sensorsPrepare(web_isLogged(request)));
}

/**
 * Restarts ESP32
 */
void web_restart(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        request->send(200, "text/plain", "OK");
        ESP.restart();
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Turns display on and off
 */
void web_dispToggle(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("num")) {
            unsigned int disp = (request->arg("num")).toInt();
            global.display_but_pressed[disp ? 1 : 0] = true;
            request->send(200, "text/plain", "OK");
        }
        else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Brightness limit
 */
void web_brightLimit(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("min") and request->hasArg("max") and request->hasArg("num")) {
            config.set_brightLimit((request->arg("min")).toInt(), (request->arg("max")).toInt(), (request->arg("num")).toInt());
            request->send(200, "text/plain", "OK");
        }
        else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Brightness adjust
 */
void web_bright(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("bright") and request->hasArg("num")) {
            config.set_bright((request->arg("bright")).toInt(), (request->arg("num")).toInt());
            request->send(200, "text/plain", "OK");
        }
        else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Ambient light sensor sensitivity adjust
 */
void web_sensitivity(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("bright") and request->hasArg("num")) {
            config.set_sensitivity((request->arg("bright")).toInt(), (request->arg("num")).toInt());
            request->send(200, "text/plain", "OK");
        }
        else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * WS2812b display timeslot color adjust
 */
void web_color(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("hex") and request->hasArg("num")) {
            char color[7];
            request->arg("hex").toCharArray(color, 6);
            config.set_color(color, (request->arg("num")).toInt(), 1);
            request->send(200, "text/plain", "OK");
        }
        else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * WS2812b display change animation
 */
void web_animation(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("num")) {
            int dispNum = request->arg("num").toInt();
            if(request->hasArg("type")) config.set_animation_type(request->arg("type").toInt(), dispNum);
            if(request->hasArg("speed")) config.set_animation_speed(request->arg("speed").toInt(), dispNum);
            if(request->hasArg("points")) config.set_animation_points(request->arg("points").toInt(), dispNum);
            request->send(200, "text/plain", "OK");
        }
        request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Initializes clock syncronization with NTP server
 */
void web_syncClock(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("ntp")) {
            global.clockSynchronized = false;
            request->send(200, "text/plain", "OK");
        }
        else {
            setTime((request->arg("h")).toInt(), 
                (request->arg("i")).toInt(),
                (request->arg("s")).toInt(),
                (request->arg("d")).toInt(),
                (request->arg("m")).toInt(),
                (request->arg("y")).toInt());
            nextion.setDisplayRTC();
            request->send(200, "text/plain", web_timeString(now()));
        }
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Sends via AJAX the information about the clock synchronization process
 */
void web_syncdialog(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(!global.clockSynchronized) request->send(200, "text/plain", ".");
        else request->send(200, "text/plain", web_timeString(now()));
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Changes the sound volume
 */
void web_soundVolume(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("vol")) {
            config.set_vol(request -> arg("vol").toInt());
            sound.volume(config.sound_vol());
            request -> send(200, "text/plain", "OK");
        }
        else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Changes the equalizer settings of the mp3 player
 */
void web_soundEqualizer(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("eq")) {
            config.set_eq(request -> arg("eq").toInt());
            sound.equalizer(config.sound_eq());
            request -> send(200, "text/plain", "OK");
        }
        else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Plays a sound track
 */
void web_soundPlay(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        if(request->hasArg("folder") and request->hasArg("track")) {
            sound.play(request -> arg("folder").toInt(), request -> arg("track").toInt());
            request -> send(200, "text/plain", "OK");
        }
        else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Stops playing a sound track
 */
void web_soundStop(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        sound.stopPlaying();
        request -> send(200, "text/plain", "OK");
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Changes password
 */
void web_changePass(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        String oldPass = (request->hasArg("oldPass")) ? request->arg("oldPass") : "";
        String newPass = (request->hasArg("newPass")) ? request->arg("newPass") : "";
        String res = "-";
        if(oldPass == config.account_pass()) {
            String json = "{\"pass\":\"";
            json += newPass;
            json += "\"}";
            File file = LittleFS.open("/user.us", FILE_WRITE);
            if(file.print(json)) res = "OK";
            else res = "ERROR Write file";
            file.close();
        }
        else res = "ERROR";
        request->send(200, "text/plain", res);
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Upload a file
 */
void web_fileUpload(AsyncWebServerRequest *request, String filename, size_t index, byte *data, size_t len, bool final) {
    if(web_isLogged(request)) {
        if(!index) request->_tempFile = LittleFS.open("/" + filename, "w");
        if(len) request->_tempFile.write(data, len);
        if(final) request->_tempFile.close();
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Delete a file
 */
void web_fileDelete(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        String path = "/" + request->arg("file");
        if(!LittleFS.exists(path)) return request->send(404, "text/plain", "FileNotFound");
        else {
            request->send(200, "text/plain", LittleFS.remove(path) ? "OK" : "ERROR");
            path = String();
        }
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}
/**
 * Rename a file
 */
void web_fileRename(AsyncWebServerRequest *request) {
    if(web_isLogged(request)) {
        String alt = "/" + request->arg("old");
        String neu = "/" + request->arg("new");
        if(!LittleFS.exists(alt)) return request->send(404, "text/plain", "FileNotFound");
        else {
            request->send(200, "text/plain", LittleFS.rename(alt, neu) ? "OK" : "ERROR");
            alt = String();
            neu = String();
        }
    }
    else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Defines the functions of the web interface
 */
void webInterface_init(void) {
    server.on("/data.json",        HTTP_GET,  [](AsyncWebServerRequest *request){ web_sens(request); });
    server.on("/esp/login",        HTTP_GET,  [](AsyncWebServerRequest *request){ web_login(request); });
    server.on("/esp/saveConfig",   HTTP_POST, [](AsyncWebServerRequest *request){ web_save(request); });
    server.on("/esp/restart",      HTTP_GET,  [](AsyncWebServerRequest *request){ web_restart(request); });
    server.on("/esp/dispToggle",   HTTP_GET,  [](AsyncWebServerRequest *request){ web_dispToggle(request); });
    server.on("/esp/brightLimit",  HTTP_GET,  [](AsyncWebServerRequest *request){ web_brightLimit(request); });
    server.on("/esp/bright",       HTTP_GET,  [](AsyncWebServerRequest *request){ web_bright(request); });
    server.on("/esp/sensitivity",  HTTP_GET,  [](AsyncWebServerRequest *request){ web_sensitivity(request); });
    server.on("/esp/animation",    HTTP_GET,  [](AsyncWebServerRequest *request){ web_animation(request); });
    server.on("/esp/color",        HTTP_GET,  [](AsyncWebServerRequest *request){ web_color(request); });
    server.on("/esp/syncClock",    HTTP_GET,  [](AsyncWebServerRequest *request){ web_syncClock(request); });
    server.on("/esp/syncdialog",   HTTP_GET,  [](AsyncWebServerRequest *request){ web_syncdialog(request); });
    server.on("/esp/changePass",   HTTP_POST, [](AsyncWebServerRequest *request){ web_changePass(request); });
    server.on("/esp/volume",       HTTP_GET,  [](AsyncWebServerRequest *request){ web_soundVolume(request); });
    server.on("/esp/equalizer",    HTTP_GET,  [](AsyncWebServerRequest *request){ web_soundEqualizer(request); });
    server.on("/esp/mp3play",      HTTP_GET,  [](AsyncWebServerRequest *request){ web_soundPlay(request); });
    server.on("/esp/mp3stop",      HTTP_GET,  [](AsyncWebServerRequest *request){ web_soundStop(request); });
    server.on("/esp/fileUpload",   HTTP_POST, [](AsyncWebServerRequest *request){ request->send(200); }, web_fileUpload);
    server.on("/description.xml",  HTTP_GET, [&](AsyncWebServerRequest *request){ request->send(200, "text/xml", SSDP.getSchema()); });
    server.on("/esp/delete",       HTTP_POST, [](AsyncWebServerRequest *request){ web_fileDelete(request); });
    server.on("/esp/rename",       HTTP_POST, [](AsyncWebServerRequest *request){ web_fileRename(request); });
    server.onNotFound([](AsyncWebServerRequest *request){ if(!web_fileRead(request)) request -> send(404); });

    MDNS.begin("bim32");

    server.begin();

    uint32_t chipId = 0;
    for(int i=0; i<17; i=i+8) {
        chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
    }

    SSDP.setSchemaURL("description.xml");
    SSDP.setHTTPPort(80);
    SSDP.setName("Weather monitor BIM32");
    SSDP.setSerialNumber(chipId);
    SSDP.setURL("/");
    SSDP.setModelName("BIM32");
    SSDP.setModelNumber(FW);
    SSDP.setModelURL("https://radiokot.ru/artfiles/6571/");
    SSDP.setManufacturer("himikat123@gmail.com");
    SSDP.setManufacturerURL("https://github.com/himikat123/Weather-monitor-BIM32");
    SSDP.setDeviceType("rootdevice");
    SSDP.setServerName("SSDPServer/1.0");
    SSDP.setIcons("<icon>"
        "<mimetype>image/png</mimetype>"
        "<height>48</height>"
        "<width>48</width>"
        "<depth>32</depth>"
        "<url>icon48.png</url>"
        "</icon>");
    SSDP.begin();

    MDNS.addService("http", "tcp", 80);
}