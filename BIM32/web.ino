RTC_DATA_ATTR double code = esp_random();
size_t content_len;

/**
 * Get current date and time as a string
 */
String web_timeString(unsigned int tm) {
  char buf[20];
  sprintf(buf, "%02d:%02d:%02d %02d.%02d.%d", hour(tm), minute(tm), second(tm), day(tm), month(tm), year(tm));
  return String(buf);
}

/**
 * Prepare sensors data
 */
String web_sensorsPrepare(bool logged) {
  char buf[20];
  StaticJsonDocument<4096> json;

  json["state"] = logged ? "OK" : "LOGIN";
  json["fw"] = global.fw;
  unsigned long mil = millis() / 1000;
  if(mil <= 86400) sprintf(buf, "%d:%02d:%02d", hour(mil), minute(mil), second(mil));
  else sprintf(buf, "%d:%02d:%02d:%02d", round(mil / 86400), hour(mil), minute(mil), second(mil));
  json["runtime"] = buf;
  json["time"] = web_timeString(now());
  json["network"]["ssid"] = global.apMode ? config.accessPoint_ssid() : WiFi.SSID();
  json["network"]["ch"] = WiFi.channel();
  json["network"]["sig"] = String(WiFi.RSSI()) + "dBm";
  json["network"]["mac"] = WiFi.macAddress();
  json["network"]["ip"] = WiFi.localIP().toString();
  json["network"]["mask"] = WiFi.subnetMask().toString();
  json["network"]["gw"] = WiFi.gatewayIP().toString();
  json["network"]["dns1"] = WiFi.dnsIP().toString();
  json["network"]["dns2"] = WiFi.dnsIP().toString();
  json["bme280"]["temp"] = round(sensors.get_bme280_temp(0) * 10) / 10;
  json["bme280"]["hum"] = round(sensors.get_bme280_hum(0) * 10) / 10;
  json["bme280"]["pres"] = round(sensors.get_bme280_pres(0) * 7.5) / 10;
  json["bmp180"]["temp"] = round(sensors.get_bmp180_temp(0) * 10) / 10;
  json["bmp180"]["pres"] = round(sensors.get_bmp180_pres(0) * 7.5) / 10;
  json["sht21"]["temp"] = round(sensors.get_sht21_temp(0) * 10) / 10;
  json["sht21"]["hum"] = round(sensors.get_sht21_hum(0) * 10) / 10;
  json["dht22"]["temp"] = round(sensors.get_dht22_temp(0) * 10) / 10;
  json["dht22"]["hum"] = round(sensors.get_dht22_hum(0) * 10) / 10;
  json["esp32"]["temp"] = round(sensors.get_esp32_temp(0) * 10) / 10;
  json["ds18b20"]["temp"] = round(sensors.get_ds18b20_temp(0) * 10) / 10;
  json["max44009"]["light"] = round(sensors.get_max44009_light(0) * 10) / 10;
  json["bh1750"]["light"] = round(sensors.get_bh1750_light(0) * 10) / 10;
  json["analog"]["volt"] = round(sensors.get_analog_voltage(0) * 100) / 100;
  json["weather"]["temp"] = round(weather.get_currentTemp() * 10) / 10;
  json["weather"]["hum"] = round(weather.get_currentHum() * 10) / 10;
  json["weather"]["pres"] = round(weather.get_currentPres() * 7.5) / 10;
  json["weather"]["wind"]["speed"] = round(weather.get_currentWindSpeed() * 10) / 10;
  json["weather"]["wind"]["dir"] = weather.get_currentWindDir();
  json["weather"]["descript"] = weather.get_description();
  for(unsigned int i=0; i<global.nets; i++) {
    json["ssids"][i][0] = global.ssids[i];
    json["ssids"][i][1] = global.rssis[i];  
  }
  for(unsigned int i=0; i<WSENSORS; i++) {
    if(wsensor.get_updated(i) == 0) json["wsensor"]["time"][i] = "--";
    else json["wsensor"]["time"][i] = web_timeString(wsensor.get_updated(i));
    for(unsigned int n=0; n<5; n++) {
      json["wsensor"]["temp"]["data"][n][i] = round(wsensor.get_temperature(i, n, 0) * 10) / 10;
      json["wsensor"]["temp"]["name"][n][i] = wsensor.get_sensorType(i, n);
    }
    json["wsensor"]["hum"]["data"][i] = round(wsensor.get_humidity(i, 0) * 10) / 10;
    json["wsensor"]["hum"]["name"][i] = wsensor.get_sensorType(i, 0);
    json["wsensor"]["pres"]["data"][i] = round(wsensor.get_pressure(i, 0) * 10) / 10;
    json["wsensor"]["pres"]["name"][i] = wsensor.get_sensorType(i, 0);
    json["wsensor"]["light"]["data"][i] = round(wsensor.get_light(i, 0) * 10) / 10;
    json["wsensor"]["light"]["name"][i] = wsensor.get_lightType(i);
    json["wsensor"]["voltage"]["data"][i] = round(wsensor.get_voltage(i, 0) * 10) / 10;
    json["wsensor"]["voltage"]["name"][i] = wsensor.get_energyType(i);
    json["wsensor"]["current"]["data"][i] = round(wsensor.get_current(i, 0) * 1000) / 1000;
    json["wsensor"]["current"]["name"][i] = wsensor.get_energyType(i);
    json["wsensor"]["power"]["data"][i] = round(wsensor.get_power(i, 0) * 10) / 10;
    json["wsensor"]["power"]["name"][i] = wsensor.get_energyType(i);
    json["wsensor"]["energy"]["data"][i] = round(wsensor.get_energy(i, 0));
    json["wsensor"]["energy"]["name"][i] = wsensor.get_energyType(i);
    json["wsensor"]["freq"]["data"][i] = round(wsensor.get_frequency(i, 0) * 10) / 10;
    json["wsensor"]["freq"]["name"][i] = wsensor.get_energyType(i);
    json["wsensor"]["bat"][i] = wsensor.get_batteryAdc(i);
  }
  File root = SPIFFS.open("/");
  if(root) {
    if(root.isDirectory()) {
      File file = root.openNextFile();
      float total = 0;
      unsigned int i = 0;
      while(file) {
        float fsize = file.size();
        String filename = String(file.name()).substring(1);
        if(filename != "user.us") {
          json["fs"]["list"][i]["name"] = filename;
          json["fs"]["list"][i++]["size"] = String(fsize / 1024);
        }
        file = root.openNextFile();
        total += fsize;
      }
      json["fs"]["total"] = 1280;
      json["fs"]["free"] = round((1280 - total / 1024) * 10) / 10;
    }
  }
  String data = "";
  serializeJsonPretty(json, data);
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
  if(!path.endsWith(".ico") && !path.endsWith(".png")) {
    if(path == "/config.json" or path == "/alarm.json") {
      if(!web_isLogged(request)) {
        request->send(200, "text/plain", "{\"lang\": \"" + config.lang() + "\", \"state\": \"LOGIN\"}");
        return true;
      }
    }
    else path = "/index.html";
  }
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if(SPIFFS.exists(pathWithGz)) path += ".gz";
    AsyncWebServerResponse *response = request->beginResponse(SPIFFS, path);
    if(request->hasArg("download")) response->addHeader("Content-Type", "application/octet-stream");
    else if(path.endsWith(".json")) response->addHeader("Content-Type", "application/json");
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
        request->send(200, "text/plain", config.save(request->arg("config")) ? "ERROR" : "OK");
      else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "ERROR");
  }
  else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Saves alarm data to alarm config file
 */
void web_saveAlarm(AsyncWebServerRequest *request) {
  if(web_isLogged(request)) {
    if(request->hasArg("alarm")) {
      if(request->arg("alarm")) 
        request->send(200, "text/plain", config.saveAlarm(request->arg("alarm")) ? "ERROR" : "OK");
      else request->send(200, "text/plain", "ERROR");
    }
    else request->send(200, "text/plain", "ERROR");
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
      if(disp == NEXTION) global.display1_but_pressed = true;
      if(disp == WS2812B) global.display2_but_pressed = true;
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
      char color[6];
      sprintf(color, "%s", request->arg("hex"));
      config.set_color(color, (request->arg("num")).toInt());
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
    if(request->hasArg("type")) config.set_animation_type((request->arg("type")).toInt());
    if(request->hasArg("speed")) config.set_animation_speed((request->arg("speed")).toInt());
    if(request->hasArg("points")) config.set_animation_points((request->arg("points")).toInt());
    request->send(200, "text/plain", "OK");
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
      File file = SPIFFS.open("/user.us", FILE_WRITE);
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
    if(!index) request->_tempFile = SPIFFS.open("/" + filename, "w");
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
    if(!SPIFFS.exists(path)) return request->send(404, "text/plain", "FileNotFound");
    else {
      SPIFFS.remove(path);
      request->send(200, "text/plain", "OK");
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
    if(!SPIFFS.exists(alt)) return request->send(404, "text/plain", "FileNotFound");
    else {
      SPIFFS.rename(alt, neu);
      request->send(200, "text/plain", "OK");
      alt = String();
      neu = String();
    }
  }
  else request->send(200, "text/plain", "NOT LOGGED");
}

/**
 * Update firmware
 */
void web_doUpdate(AsyncWebServerRequest *request, const String& filename, size_t index, byte *data, size_t len, bool final) {
  if(web_isLogged(request)) {
    global.stopOS = true;
    if(!index) {
      content_len = request->contentLength();
      int cmd = (filename.indexOf("fs") > -1) ? U_SPIFFS : U_FLASH;
      if(!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) Update.printError(Serial);
    }
    if(Update.write(data, len) != len) Update.printError(Serial);
    if(final) {
      AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "wait");
      response->addHeader("Refresh", "20");  
      response->addHeader("Location", "/");
      request->send(response);
      if(!Update.end(true)) Update.printError(Serial);
      else ESP.restart();
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
  server.on("/esp/saveAlarm",    HTTP_POST, [](AsyncWebServerRequest *request){ web_saveAlarm(request); });
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
  server.on("/description.xml",  HTTP_GET, [&](AsyncWebServerRequest *request){ request->send(200, "text/xml", SSDP.schema(false)); });
  server.on("/esp/delete",       HTTP_POST, [](AsyncWebServerRequest *request){ web_fileDelete(request); });
  server.on("/esp/rename",       HTTP_POST, [](AsyncWebServerRequest *request){ web_fileRename(request); });
  server.on("/esp/fwUpdate",     HTTP_POST, [](AsyncWebServerRequest *request){},
    [](AsyncWebServerRequest *request, const String& filename,
    size_t index, byte *data, size_t len, bool final) {
      web_doUpdate(request, filename, index, data, len, final);
  });
  server.onNotFound([](AsyncWebServerRequest *request){ if(!web_fileRead(request)) request -> send(404); });

  MDNS.begin("bim32");
  
  server.begin();

  uint32_t chipId = 0;
  for(int i=0; i<17; i=i+8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName("Weather monitor");
  SSDP.setSerialNumber(chipId);
  SSDP.setURL("/");
  SSDP.setModelName("BIM32");
  SSDP.setModelNumber(String(global.fw));
  SSDP.setModelURL("https://github.com/himikat123/Weather-monitor-BIM32");
  SSDP.setManufacturer("himikat123@gmail.com");
  SSDP.setManufacturerURL("https://radiokot.ru/artfiles/6571/");
  SSDP.setDeviceType("rootdevice");
  SSDP.setServerName("SSDPServer/1.0");
  SSDP.setIcons("<icon>"
                  "<mimetype>image/png</mimetype>"
                  "<height>48</height>"
                  "<width>48</width>"
                  "<depth>24</depth>"
                  "<url>icon48.png</url>"
                "</icon>"
  );
  SSDP.begin();

  MDNS.addService("http", "tcp", 80);
}
