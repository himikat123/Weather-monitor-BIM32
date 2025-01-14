RTC_DATA_ATTR double secretCode = esp_random();
size_t content_len;
String web_filelist = "";
size_t fsUsed = 0;
size_t fsTotal = 0;
File fsUploadFile;

/**
 * Get current date and time as a string
 */
String web_timeString(unsigned int tm) {
    char buf[20];
    sprintf(buf, "%02d:%02d:%02d %02d.%02d.%d", hour(tm), minute(tm), second(tm), day(tm), month(tm), year(tm));
    return String(buf);
}

/**
 * Update filelist
 */
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
 * Check if the user is logged in
 */
bool web_isLogged(bool answer) {
    bool logged = false;
    if(!config.account_required()) logged = true;
    else {
        if(server.hasArg("code")) {
            String auth = server.arg("code");
            if(auth == String(secretCode)) logged = true;
            else logged = false;
        }
        else logged = false;
    }

    if(!logged && answer) server.send(200, "text/plain", "NOT LOGGED IN");
    return logged;
}

String web_getContentType(String filename) {
  if(server.hasArg("download"))       return "application/octet-stream";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".json")) return "text/json";
  else if(filename.endsWith(".jpg"))  return "image/jpeg";
  else if(filename.endsWith(".png"))  return "image/png";
  else if(filename.endsWith(".css"))  return "text/css";
  else if(filename.endsWith(".js"))   return "application/javascript";
  else if(filename.endsWith(".gz"))   return "application/x-gzip";
  return "text/plain";
}

/**
 * Read a file
 */
bool web_fileRead(String path) {
    String contentType = web_getContentType(path);
    String pathWithGz = path + ".gz";

    if(LittleFS.exists(pathWithGz) || LittleFS.exists(path)) {
        if(LittleFS.exists(pathWithGz)) path += ".gz";
        File file = LittleFS.open(path, "r");
        if(path.endsWith(".js.gz") || path.endsWith(".css.gz")) {
            server.sendHeader("Cache-Control", "max-age=31536000");
        }
        server.streamFile(file, contentType);
        file.close();
        return true;
    }
    return false;
}

/**
 * Check if the user is logged in and sends the requested file, otherwise sends the login page
 */
bool web_getFile(String path) {
    if(path.endsWith(".json") or path.endsWith(".jpg") or path.endsWith(".png")) {
        if(!web_isLogged(false)) {
            if(path.endsWith(".json")) {
                server.send(200, "application/json", "{\"lang\": \"" + config.lang() + "\", \"state\": \"LOGIN\"}");
                return true;
            }
        }
    }
    else {
        if(!path.endsWith(".css") && !path.endsWith(".js")) path = "/index.html";
    }
    return web_fileRead(path);
}

/**
 * Login
 * Username and password validation 
 */
void web_login() {
    String user = "", pass = "";
    if(server.hasArg("name")) user = server.arg("name");
    if(server.hasArg("pass")) pass = server.arg("pass");
    bool loged = false;
    if(user == config.account_name() and pass == config.account_pass()) {
        loged = true;
        secretCode = esp_random();
    }
    server.send(200, "text/plain", loged ? ("OK:" + String(round(secretCode))) : "error:1");
}

/**
 * Save config data to config file
 */
void web_save() {
    if(web_isLogged(true)) {
        if(server.hasArg("config")) {
            String cfg = server.arg("config");
            if(cfg.length() > 0) {
                if(cfg.lastIndexOf("\"ssid\":") != -1) {
                    bool err = true;
                    File file = LittleFS.open("/config.json", "w");
                    if(file) err = !file.print(cfg);
                    file.close();
                    global.fsInfoUpdate = true;
                    server.send(200, "text/plain", err ? "SAVE ERROR" : "OK");
                }
                else server.send(200, "text/plain", "CONFIG ERROR");
            }
            else server.send(200, "text/plain", "CONFIG ARGUMENT EMPTY");
            cfg = String();
            config.readConfig();
            // не хватает памяти
        }
        else server.send(200, "text/plain", "NO CONFIG ARGUMENT");
    }
}

/**
 * Save alarms data to alarm file
 */
void web_save_alarm() {
    if(web_isLogged(true)) {
        if(server.hasArg("alarm")) {
            String alr = server.arg("alarm");
            if(alr.length() > 0) {
                if(alr.lastIndexOf("\"states\":") != -1) {
                    bool err = true;
                    File file = LittleFS.open("/alarm.json", "w");
                    if(file) err = !file.print(alr);
                    file.close();
                    global.fsInfoUpdate = true;
                    server.send(200, "text/plain", err ? "SAVE ERROR" : "OK");
                }
                else server.send(200, "text/plain", "ALARM ERROR");
            }
            else server.send(200, "text/plain", "ALARM ARGUMENT EMPTY");
            alr = String();
            config.readConfig();
        }
        else server.send(200, "text/plain", "NO ALARM ARGUMENT");
    }
}

/**
 * Send sensors data via AJAX
 */
void web_sens() {
    JsonDocument json;
    json["state"] = web_isLogged(false) ? "OK" : "LOGIN";
    json["fw"] = FW;
    json["runtime"] = round(millis() / 1000);
    json["heap"] = ESP.getFreeHeap();
    json["time"] = now();

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

    json["bme280"]["temp"] = sensors.get_bme280_temp(RAW);
    json["bme280"]["hum"] = sensors.get_bme280_hum(RAW);
    json["bme280"]["pres"] = sensors.get_bme280_pres(RAW);
    json["bmp180"]["temp"] = sensors.get_bmp180_temp(RAW);
    json["bmp180"]["pres"] = sensors.get_bmp180_pres(RAW);
    json["sht21"]["temp"] = sensors.get_sht21_temp(RAW);
    json["sht21"]["hum"] = sensors.get_sht21_hum(RAW);
    json["dht22"]["temp"] = sensors.get_dht22_temp(RAW);
    json["dht22"]["hum"] = sensors.get_dht22_hum(RAW);
    json["esp32"]["temp"] = sensors.get_esp32_temp(RAW);
    json["ds18b20"]["temp"] = sensors.get_ds18b20_temp(RAW);
    json["max44009"]["light"] = sensors.get_max44009_light(RAW);
    json["bh1750"]["light"] = sensors.get_bh1750_light(RAW);
    json["analog"]["volt"] = sensors.get_analog_voltage(RAW);
    json["bme680"]["temp"] = sensors.get_bme680_temp(RAW);
    json["bme680"]["hum"] = sensors.get_bme680_hum(RAW);
    json["bme680"]["pres"] = sensors.get_bme680_pres(RAW);
    json["bme680"]["iaq"] = sensors.get_bme680_iaq(RAW);
    json["bme680"]["iaqAccr"] = sensors.get_bme680_iaq_accuracy();

    json["thing"]["time"] = thingspeak.get_updated();
    for(unsigned int i=0; i<THNG_FIELDS; i++) {
        json["thing"]["data"][i] = thingspeak.get_field(i);
    }

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

    if(global.fsInfoUpdate) {
        web_filelist = String();
        web_listAllFilesInDir("/");
        fsTotal = LittleFS.totalBytes();
        fsUsed = fsTotal - LittleFS.usedBytes();
        global.fsInfoUpdate = false;
    }
    json["fs"]["list"] = web_filelist;
    json["fs"]["total"] = fsTotal;
    json["fs"]["free"] = fsUsed;

    String data = "";
    serializeJson(json, data);

    server.send(200, "application/json", data);
}

/**
 * Restart ESP32
 */
void web_restart() {
    if(web_isLogged(true)) {
        server.send(200, "text/plain", "OK");
        ESP.restart();
    }
}

/* Scan available networks */
void web_netlist() {
    if(web_isLogged(true)) {
        network.scanNetworks();
        server.send(200, "text/plain", "OK");
    }
}

/**
 * Change language
 */
void web_changeLang() {
    if(web_isLogged(true)) {
        if(server.hasArg("lang")) {
            config.set_lang(server.arg("lang"));
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "error");
    }
}

/**
 * Turn display on and off
 */
void web_dispToggle() {
    if(web_isLogged(true)) {
        if(server.hasArg("num")) {
            uint8_t dispNum = (server.arg("num")).toInt();
            global.display_but_pressed[dispNum] = true;
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Change brightness limit
 */
void web_brightLimit() {
    if(web_isLogged(true)) {
        if(server.hasArg("min") and server.hasArg("max") and server.hasArg("num")) {
            config.set_brightLimit(
                (server.arg("min")).toInt(), 
                (server.arg("max")).toInt(), 
                (server.arg("num")).toInt()
            );
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Brightness adjust
 */
void web_bright() {
    if(web_isLogged(true)) {
        if(server.hasArg("bright") and server.hasArg("num")) {
            config.set_bright((server.arg("bright")).toInt(), (server.arg("num")).toInt());
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Ambient light sensor sensitivity adjust
 */
void web_sensitivity() {
    if(web_isLogged(true)) {
        if(server.hasArg("bright") and server.hasArg("num")) {
            config.set_sensitivity((server.arg("bright")).toInt(), (server.arg("num")).toInt());
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Display timeslot color adjust
 */
void web_color() {
    if(web_isLogged(true)) {
        if(server.hasArg("hex") and server.hasArg("slot") and server.hasArg("num")) {
            char color[7];
            server.arg("hex").toCharArray(color, 7);
            config.set_color(color, (server.arg("slot")).toInt(), (server.arg("num")).toInt());
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Display change animation
 */
void web_animation() {
    if(web_isLogged(true)) {
        if(server.hasArg("num")) {
            int dispNum = server.arg("num").toInt();
            if(server.hasArg("type")) config.set_animation_type(server.arg("type").toInt(), dispNum);
            if(server.hasArg("speed")) config.set_animation_speed(server.arg("speed").toInt(), dispNum);
            if(server.hasArg("points")) config.set_animation_points(server.arg("points").toInt(), dispNum);
            server.send(200, "text/plain", "OK");
        }
        server.send(200, "text/plain", "ERROR");
    }
}

/*
 * ILI9341 display touch calibrate
 */
void web_touch_calibrate() {
    if(web_isLogged(true)) {
        global.touch_calibrate = true;
        server.send(200, "text/plain", "OK");
    }
}

/**
 * Initialize clock syncronization with NTP server
 */
void web_syncClock() {
    if(web_isLogged(true)) {
        if(server.hasArg("ntp")) {
            global.clockSynchronized = false;
            server.send(200, "text/plain", "OK");
        }
        else {
            setTime(
                (server.arg("h")).toInt(), 
                (server.arg("i")).toInt(),
                (server.arg("s")).toInt(),
                (server.arg("d")).toInt(),
                (server.arg("m")).toInt(),
                (server.arg("y")).toInt()
            );
            nextion.setDisplayRTC();
            sensors.set_ds3231_timeDate();
            server.send(200, "text/plain", web_timeString(now()));
        }
    }
}

/**
 * Send via AJAX the information about the clock synchronization process
 */
void web_syncdialog() {
    if(web_isLogged(true)) {
        if(!global.clockSynchronized) server.send(200, "text/plain", ".");
        else server.send(200, "text/plain", web_timeString(now()));
    }
}

/**
 * Change sound volume
 */
void web_soundVolume() {
    if(web_isLogged(true)) {
        if(server.hasArg("vol")) {
            config.set_vol(server.arg("vol").toInt());
            sound.volume(config.sound_vol());
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Change equalizer settings of the mp3 player
 */
void web_soundEqualizer() {
    if(web_isLogged(true)) {
        if(server.hasArg("eq")) {
            config.set_eq(server.arg("eq").toInt());
            sound.equalizer(config.sound_eq());
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Play a sound track
 */
void web_soundPlay() {
    if(web_isLogged(true)) {
        if(server.hasArg("folder") and server.hasArg("track")) {
            sound.play(server.arg("folder").toInt(), server.arg("track").toInt());
            server.send(200, "text/plain", "OK");
        }
        else server.send(200, "text/plain", "ERROR");
    }
}

/**
 * Stop playing a sound track
 */
void web_soundStop() {
    if(web_isLogged(true)) {
        sound.stopPlaying();
        server.send(200, "text/plain", "OK");
    }
}

/**
 * Restore default config
 */
void web_default() {
    if(web_isLogged(true)) {
        if(server.hasArg("config") && server.arg("config") == "default") {
            char ibuffer[64];
            if(LittleFS.exists("/config.json") == true) LittleFS.remove("/config.json");
            File def = LittleFS.open("/defaultConfig.json", "r");
            File cfg = LittleFS.open("/config.json", "w");
            if(def and cfg) {
                while(def.available()) {
                    byte i = def.readBytes(ibuffer, 64);
                    cfg.write((uint8_t *)ibuffer, i);
                }
                server.send(200, "text/plain", "OK");
            }
            cfg.close();
            def.close();
            global.fsInfoUpdate = true;
            server.send(200, "text/plain", "error");
        }
        else server.send(200, "text/plain", "bad args");
    }
}

/**
 * Change password
 */
void web_changePass() {
    if(web_isLogged(true)) {
        String oldPass = (server.hasArg("oldPass")) ? server.arg("oldPass") : "";
        String newPass = (server.hasArg("newPass")) ? server.arg("newPass") : "";
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
        server.send(200, "text/plain", res);
    }
}

/**
 * Upload a file
 */
void web_fileUpload() {
    HTTPUpload& upload = server.upload();
    if(upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;
        if(!filename.startsWith("/")) filename = "/" + filename;
        fsUploadFile = LittleFS.open(filename, "w");
        filename = String();
    }
    else if(upload.status == UPLOAD_FILE_WRITE) {
        if(fsUploadFile) fsUploadFile.write(upload.buf, upload.currentSize);
    }
    else if(upload.status == UPLOAD_FILE_END) {
        global.fsInfoUpdate = true;
        if(fsUploadFile) fsUploadFile.close();
    }
}

/**
 * Delete a file
 */
void web_fileDelete() {
    if(web_isLogged(true)) {
        String path = "/" + server.arg("file");
        if(!LittleFS.exists(path)) return server.send(404, "text/plain", "FileNotFound");
        else {
            server.send(200, "text/plain", LittleFS.remove(path) ? "OK" : "ERROR");
            path = String();
            global.fsInfoUpdate = true;
        }
    }
}

/**
 * Rename a file
 */
void web_fileRename() {
    if(web_isLogged(true)) {
        String alt = "/" + server.arg("old");
        String neu = "/" + server.arg("new");
        if(!LittleFS.exists(alt)) return server.send(404, "text/plain", "FileNotFound");
        else {
            server.send(200, "text/plain", LittleFS.rename(alt, neu) ? "OK" : "ERROR");
            alt = String();
            neu = String();
            global.fsInfoUpdate = true;
        }
    }
}

/**
 * Defines the functions of the web interface
 */
void webInterface_init(void) {
    server.on("/data.json",         HTTP_GET,  web_sens);
    server.on("/esp/login",         HTTP_POST, web_login);
    server.on("/esp/saveConfig",    HTTP_POST, web_save);
    server.on("/esp/saveAlarm",     HTTP_POST, web_save_alarm);
    server.on("/esp/restart",       HTTP_GET,  web_restart);
    server.on("/esp/netlist",       HTTP_GET,  web_netlist);
    server.on("/esp/changelang",    HTTP_GET,  web_changeLang);
    server.on("/esp/dispToggle",    HTTP_GET,  web_dispToggle);
    server.on("/esp/brightLimit",   HTTP_GET,  web_brightLimit);
    server.on("/esp/bright",        HTTP_GET,  web_bright);
    server.on("/esp/sensitivity",   HTTP_GET,  web_sensitivity);
    server.on("/esp/animation",     HTTP_GET,  web_animation);
    server.on("/esp/color",         HTTP_GET,  web_color);
    server.on("/esp/calibrate",     HTTP_GET,  web_touch_calibrate);
    server.on("/esp/syncClock",     HTTP_GET,  web_syncClock);
    server.on("/esp/syncdialog",    HTTP_GET,  web_syncdialog);
    server.on("/esp/changePass",    HTTP_POST, web_changePass);
    server.on("/esp/volume",        HTTP_GET,  web_soundVolume);
    server.on("/esp/equalizer",     HTTP_GET,  web_soundEqualizer);
    server.on("/esp/mp3play",       HTTP_GET,  web_soundPlay);
    server.on("/esp/mp3stop",       HTTP_GET,  web_soundStop);
    server.on("/esp/defaultConfig", HTTP_POST, web_default);
    server.on("/esp/fileUpload",    HTTP_POST, [] () { server.send(200, "text/plain", ""); }, web_fileUpload);
    server.on("/description.xml",   HTTP_GET,  [] () { SSDP.getSchema(); });
    server.on("/esp/delete",        HTTP_POST, web_fileDelete);
    server.on("/esp/rename",        HTTP_POST, web_fileRename);
    server.onNotFound([]() { if(!web_getFile(server.uri())) { server.send(404, "text/plain", "FileNotFound"); }});

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
    SSDP.setIcons(
        "<icon>"
            "<mimetype>image/png</mimetype>"
            "<height>48</height>"
            "<width>48</width>"
            "<depth>32</depth>"
            "<url>icon48.png</url>"
        "</icon>");
    SSDP.begin();

    MDNS.addService("http", "tcp", 80);
}
