size_t content_len;
bool loged = false;
RTC_DATA_ATTR double code = esp_random();
long lastMsg = 0;
char msg[50];
int value = 0;

void handleDoUpdate(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final){
  if(!index){
    content_len = request -> contentLength();
    int cmd = (filename.indexOf("fs") > -1) ? U_SPIFFS : U_FLASH;
    if(!Update.begin(UPDATE_SIZE_UNKNOWN, cmd)) Update.printError(Serial);
  }
  if(Update.write(data, len) != len) Update.printError(Serial);
  if(final){
    AsyncWebServerResponse *response = request -> beginResponse(302, "text/plain", "wait");
    response -> addHeader("Refresh", "20");  
    response -> addHeader("Location", "/");
    request -> send(response);
    if(!Update.end(true)) Update.printError(Serial);
    else ESP.restart();
  }
}

class GetCookieValue{
  public:
    String SearchString, StringName, CookieValue;
    int PozStart, Length, PozStop;

  void Set(String ss){
    SearchString = ss;
  }
 
  String Get(String sn){
    StringName = sn + "=";
    Length = StringName.length();
    PozStart = SearchString.indexOf(StringName) + Length;
    PozStop = SearchString.indexOf(";", PozStart);
    CookieValue = SearchString.substring(PozStart, PozStop); 
    return CookieValue;
  }
};

void web_only(bool w){
  String json;
  DynamicJsonDocument conf(64);
  conf["web"] = w ? 1 : 0;
  serializeJson(conf, json);
  File file = SPIFFS.open("/web.json", FILE_WRITE);
  file.print(json);
  file.close();
}

bool handleFileRead(AsyncWebServerRequest *request){
  String path = request -> url();  
  if(request -> hasHeader("Cookie")){
    AsyncWebHeader* h = request -> getHeader("Cookie");
    GetCookieValue c;
    c.Set(h -> value().c_str());
    String auth = c.Get("auth");
    if(auth == String(code)){
      if(datas.ap_mode){
        if(path == "/jquery.js" or path == "/favicon.ico" or path == "/config.json") return FileRead(request, path);
        else return FileRead(request, "/apnetw.htm");
      }
      else return FileRead(request, path);
    }
    else{
      if(!datas.ap_mode) web_only(true);
      if(!datas.ap_mode) if(!config.web_s) ESP.restart();
      if(loged){
        if(datas.ap_mode) return FileRead(request, "/apnetw.htm");
        else return FileRead(request, "/");
      }
      else return FileRead(request, "/login.htm");
    }
  }
  else{
    if(!datas.ap_mode) web_only(true);
    if(!datas.ap_mode) if(!config.web_s) ESP.restart();
    if(loged) return FileRead(request, "/");
    else return FileRead(request, "/login.htm");
  }
}

bool FileRead(AsyncWebServerRequest *request, String path){
  if(path.endsWith("/")) path += "index.htm";
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz)) path += ".gz";
    AsyncWebServerResponse *response = request -> beginResponse(SPIFFS, path);
    if(request -> hasArg("download")) response -> addHeader("Content-Type", "application/octet-stream");
    else if(path.endsWith(".htm"))  response -> addHeader("Content-Type", "text/html");
    else if(path.endsWith(".css"))  response -> addHeader("Content-Type", "text/css");
    else if(path.endsWith(".js"))   response -> addHeader("Content-Type", "application/javascript");
    else if(path.endsWith(".json")) response -> addHeader("Content-Type", "application/json");
    else if(path.endsWith(".png"))  response -> addHeader("Content-Type", "image/png");
    else if(path.endsWith(".gif"))  response -> addHeader("Content-Type", "image/gif");
    else if(path.endsWith(".jpg"))  response -> addHeader("Content-Type", "image/jpeg");
    else if(path.endsWith(".ico"))  response -> addHeader("Content-Type", "image/x-icon");
    else if(path.endsWith(".gz"))   response -> addHeader("Content-Encoding", "gzip");
    else response -> addHeader("Content-Type", "text/plain");
    if(loged){
      if(path.endsWith(".htm")){
        loged = false;
        response -> addHeader("Set-Cookie", "auth=" + String(code));
      }
    }
    request -> send(response);
    return true;
  }
  return false;
}

void web_interface_init(void){
  ArduinoOTA.onStart([](){events.send("Update Start", "ota");});
  ArduinoOTA.onEnd([](){events.send("Update End", "ota");});
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total){
    char p[32];
    sprintf(p, "Progress: %u%%\n", (progress / (total / 100)));
    events.send(p, "ota");
  });
  ArduinoOTA.onError([](ota_error_t error){
    if(error == OTA_AUTH_ERROR) events.send("Auth Failed", "ota");
    else if(error == OTA_BEGIN_ERROR) events.send("Begin Failed", "ota");
    else if(error == OTA_CONNECT_ERROR) events.send("Connect Failed", "ota");
    else if(error == OTA_RECEIVE_ERROR) events.send("Recieve Failed", "ota");
    else if(error == OTA_END_ERROR) events.send("End Failed", "ota");
  });
  ArduinoOTA.setHostname("BIM32");
  ArduinoOTA.begin();
  server.addHandler(&events);
  server.addHandler(new SPIFFSEditor(SPIFFS));
  ///////////////////////////////////////////////////////////////////////////////////
  server.on("/esp/login.php", HTTP_POST, [](AsyncWebServerRequest *request){
    String login = request -> arg("LOGIN");
    String pass = request -> arg("PASSW");
    if(login == config.username and pass == config.password){
      loged = true;
      code = esp_random();
    }
    else loged = false;
    AsyncWebServerResponse *response = request -> beginResponse(301);
    response -> addHeader("Location", (loged) ? "/" : "/login.htm?error");
    response -> addHeader("Cache-Control", "no-cache");
    response -> addHeader("Set-Cookie", "auth=" + (loged) ? String(code) : "0");
    request -> send(response);
  });
  ////////////////////////////////////////////////////////////////////////////////////
  server.on("/esp/logout.php", HTTP_POST, [](AsyncWebServerRequest *request){
    web_only(false);
    request -> send(200, "text/plain", "OK");
    ESP.restart();
  });
  ////////////////////////////////////////////////////////////////////////////////////
  server.on("/esp/save.php", HTTP_POST, [](AsyncWebServerRequest *request){
    String daten = request -> arg("CONFIG");
    File file=SPIFFS.open("/config.json", "w");
    if(file){
      file.print(daten);
      file.close();
      request -> send(200, "text/plain", "OK");
    }
    else request -> send(200, "text/plain", "ERROR");
  });
  ////////////////////////////////////////////////////////////////////////////////////
  server.on("/esp/status.php", HTTP_GET, [](AsyncWebServerRequest *request){
    String answ = "{\"fw\":\""; answ += fw;
    answ += "\",\"int_temp\":\""; answ += String((temprature_sens_read() - 32) / 1.8) + "°C";
    unsigned long mil = millis() / 1000;
    String seconds = ((mil % 60) < 10) ? "0" + String(mil % 60) : String(mil % 60);
    String minutes = (((mil / 60) % 60) < 10) ? "0" + String((mil / 60) % 60) : String((mil / 60) % 60);
    String hours = (((mil / 3600) % 24) < 10) ? "0" + String((mil / 3600) % 24) : String((mil / 3600) % 24);
    answ += "\",\"millis\":\""; answ += hours + ":" + minutes + ":" + seconds;
    answ += "\",\"ssid\":\""; answ += WiFi.SSID();
    answ += "\",\"ch\":\""; answ += WiFi.channel();
    answ += "\",\"sig\":\""; answ += String(WiFi.RSSI()) + "dB";
    answ += "\",\"mac\":\""; answ += WiFi.macAddress();
    answ += "\",\"ip\":\""; answ += WiFi.localIP().toString();
    answ += "\",\"dbmet\":\""; answ += sensors.bme280_det ? String(sensors.bme280_temp) : "40400";
    answ += "\",\"dbmeh\":\""; answ += sensors.bme280_det ? String(sensors.bme280_hum) : "40400";
    answ += "\",\"dbmep\":\""; answ += sensors.bme280_det ? String(sensors.bme280_pres) : "40400";
    answ += "\",\"dbmpt\":\""; answ += sensors.bmp180_det ? String(sensors.bmp180_temp) : "40400";
    answ += "\",\"dbmpp\":\""; answ += sensors.bmp180_det ? String(sensors.bmp180_pres) : "40400";
    answ += "\",\"dshtt\":\""; answ += sensors.sht21_det ? String(sensors.sht21_temp) : "40400";
    answ += "\",\"dshth\":\""; answ += sensors.sht21_det ? String(sensors.sht21_hum) : "40400";
    answ += "\",\"ddhtt\":\""; answ += sensors.dht22_det ? String(sensors.dht22_temp) : "40400";
    answ += "\",\"ddhth\":\""; answ += sensors.dht22_det ? String(sensors.dht22_hum) : "40400";
    answ += "\",\"dmax\":\""; answ += sensors.max44009_det ? String(sensors.max44009_light) : "40400";
    answ += "\",\"dbh\":\""; answ += sensors.bh1750_det ? String(sensors.bh1750_light) : "40400";
    answ += "\",\"temp_wsens\":\""; answ += String(datas.temp_wsens + config.wsens_temp_corr);
    answ += "\",\"hum_wsens\":\""; answ += String(datas.hum_wsens + config.wsens_hum_corr);
    answ += "\",\"pres_wsens\":\""; answ += String(datas.pres_wsens + config.wsens_pres_corr);
    answ += "\",\"light_wsens\":\""; answ += String(datas.light_wsens + config.wsens_light_corr);
    answ += "\",\"wbat_level\":\""; answ += String(datas.wbat_level);
    answ += "\",\"wbat_voltage\":\""; answ += String(datas.wbat_voltage);
    answ += "\"}";
    request -> send(200, "text/json", answ);
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/wsensor.php", HTTP_GET, [](AsyncWebServerRequest *request){
    if(year() < 2021){
      setTime((request -> arg("H")).toInt(),
              (request -> arg("I")).toInt(),
              (request -> arg("S")).toInt(),
              (request -> arg("D")).toInt(),
              (request -> arg("M")).toInt(),
              (request -> arg("Y")).toInt()
      );
    }
    String answ = "{";
    answ += "\"wtemp\":"; answ += String(datas.temp_wsens);
    answ += ",\"whum\":"; answ += String(datas.hum_wsens);
    answ += ",\"wpres\":"; answ += String(datas.pres_wsens);
    answ += ",\"wlight\":"; answ += String(datas.light_wsens);
    answ += ",\"adc\":"; answ += String(datas.bat_adc);
    answ += ",\"wtemptp\":\""; answ += datas.wtemp_sens;
    answ += "\",\"whumtp\":\""; answ += datas.whum_sens;
    answ += "\",\"wprestp\":\""; answ += datas.wpres_sens;
    answ += "\",\"wlighttp\":\""; answ += datas.wlight_sens;
    char recieve_time[40];
    sprintf(recieve_time, 
      "%u:%02u:%02u %02u-%02u-%u",
      hour(datas.time_wsens), 
      minute(datas.time_wsens),
      second(datas.time_wsens),
      month(datas.time_wsens),
      day(datas.time_wsens),
      year(datas.time_wsens)
    );
    answ += "\",\"time\":\""; answ += String(recieve_time);
    answ += "\"}";
    request -> send(200, "text/json", answ);
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/ssids.php", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    uint8_t n = WiFi.scanNetworks();
    for(uint8_t i=0; i<n; i++){
      json += "\""; 
      json += WiFi.SSID(i);
      json += "\":\"";
      json += abs(WiFi.RSSI(i));
      if(i == n - 1) json += "\"}";
      else json += "\",";
    }
    request -> send(200, "text/json", json);
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/ip_gw.php", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"ip_s\":\""; json += WiFi.localIP().toString();    json += "\",";
    json += "\"gw_s\":\"";        json += WiFi.gatewayIP().toString();  json += "\",";
    json += "\"mask_s\":\"";      json += WiFi.subnetMask().toString(); json += "\"}";
    request -> send(200, "text/json", json);
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/sensors.php", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"dbmet\":\""; json += sensors.bme280_det ? String(sensors.bme280_temp - sensors.bme280_temp_corr) : "40400"; json += "\",";
    json += "\"dbmeh\":\""; json += sensors.bme280_det ? String(sensors.bme280_hum - sensors.bme280_hum_corr) : "40400"; json += "\",";
    json += "\"dbmep\":\""; json += sensors.bme280_det ? String(sensors.bme280_pres - sensors.bme280_pres_corr) : "40400"; json += "\",";
    json += "\"dbmpt\":\""; json += sensors.bmp180_det ? String(sensors.bmp180_temp - sensors.bmp180_temp_corr) : "40400"; json += "\",";
    json += "\"dbmpp\":\""; json += sensors.bmp180_det ? String(sensors.bmp180_pres - sensors.bmp180_pres_corr) : "40400"; json += "\",";
    json += "\"dshtt\":\""; json += sensors.sht21_det ? String(sensors.sht21_temp - sensors.sht21_temp_corr) : "40400"; json += "\",";
    json += "\"dshth\":\""; json += sensors.sht21_det ? String(sensors.sht21_hum - sensors.sht21_hum_corr) : "40400"; json += "\",";
    json += "\"ddhtt\":\""; json += sensors.dht22_det ? String(sensors.dht22_temp - sensors.dht22_temp_corr) : "40400"; json += "\",";
    json += "\"ddhth\":\""; json += sensors.dht22_det ? String(sensors.dht22_hum - sensors.dht22_hum_corr) : "40400"; json += "\",";
    json += "\"dmax\":\""; json += sensors.max44009_det ? String(sensors.max44009_light - sensors.max44009_light_corr) : "40400"; json += "\",";
    json += "\"dbh\":\""; json += sensors.bh1750_det ? String(sensors.bh1750_light - sensors.bh1750_light_corr) : "40400"; json += "\"}";
    request -> send(200, "text/json", json);
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/bright.php", HTTP_GET, [](AsyncWebServerRequest *request){
    int bright = (request -> arg("br")).toInt();
    myNex.writeNum("dim", bright);
    request -> send(200, "text/plain", "OK");
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/ws_bright.php", HTTP_GET, [](AsyncWebServerRequest *request){
    int bright = (request -> arg("br")).toInt();
    datas.bright_clock = bright;
    request -> send(200, "text/plain", "OK");
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/sensitivity.php", HTTP_GET, [](AsyncWebServerRequest *request){
    int sensitivity = (request -> arg("sen")).toInt();
    float light = 100.0;
    switch(config.light_in){
      case 1: light = datas.light_wsens; break;
      case 6: light = sensors.max44009_light; break;
      case 7: light = sensors.bh1750_light; break;
      default: light = 100.0; break;
    }
    light *= (float)sensitivity / 10.0;
    if(light < 1.0) light = 1.0;
    if(light > 100.0) light = 100.0;
    myNex.writeNum("dim", round(light));
    request -> send(200, "text/plain", "OK");
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/ws_sensitivity.php", HTTP_GET, [](AsyncWebServerRequest *request){
    int sensitivity = (request -> arg("sen")).toInt();
    float light = 100.0;
    switch(config.ws_light_in){
      case 1: light = datas.light_wsens; break;
      case 6: light = sensors.max44009_light; break;
      case 7: light = sensors.bh1750_light; break;
      default: light = 100.0; break;
    }
    light *= (float)sensitivity / 10.0;
    if(light < 1.0) light = 1.0;
    if(light > 100.0) light = 100.0;
    datas.bright_clock = round(light);
    request -> send(200, "text/plain", "OK");
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/brt_sen.php", HTTP_GET, [](AsyncWebServerRequest *request){
    int sens = (request -> arg("brt")).toInt();
    config.light_in = sens;
    request -> send(200, "text/plain", "OK");
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/ws_brt_sen.php", HTTP_GET, [](AsyncWebServerRequest *request){
    int sens = (request -> arg("brt")).toInt();
    datas.bright_clock = sens;
    request -> send(200, "text/plain", "OK");
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/mac.php", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{\"mac\":\"";
    json +=  WiFi.macAddress();
    json += "\"}";
    request -> send(200, "text/json", json);
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/username.php", HTTP_GET, [](AsyncWebServerRequest *request){
    String username = "{\"user\":\"";
    username += config.username;
    username += "\"}";
    request -> send(200, "text/json", username);
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/user.php", HTTP_POST, [](AsyncWebServerRequest *request){
    String username = request -> arg("name");
    String oldpass = request -> arg("oldpass");
    String newpass = request -> arg("newpass");
    String json = "{\"user\":\"";
    json += username;
    json += "\",\"pass\":\"";
    json += newpass;
    json += "\"}";
    String res = "-";
    if(oldpass == config.password){
      username.toCharArray(config.username, sizeof(config.username));
      newpass.toCharArray(config.password, sizeof(config.password));
      File file = SPIFFS.open("/user.us", FILE_WRITE);
      if(file.print(json)) res = "OK";
      else res = "ERROR Write file";
      file.close();
    }
    else res = "error";
    request -> send(200, "text/plain", res);
  });
  //////////////////////////////////////////////////////////////////////////
  server.on("/esp/list.php", HTTP_GET, [](AsyncWebServerRequest *request){
    File root = SPIFFS.open("/");
    if(!root) return;
    if(!root.isDirectory()) return;
    File file = root.openNextFile();
    String output = "{\"fl\":[";
    while(file){
      if(output != "{\"fl\":[") output += ',';
      String nm = file.name();
      output += "{\"name\":\""; output += nm;
      output += "\",\"type\":\"file\",\"size\":\""; output += file.size();
      output += "\"}";
      file = root.openNextFile();
    }
    root = SPIFFS.open("/");
    file = root.openNextFile();
    output += "],\"fs\":{";
    output += "\"total\":1280,\"used\":";
    float used = 0;
    while(file){
      used += file.size();
      file = root.openNextFile();
    }
    output += String(round(used / 1024));
    output += "}}";
    request -> send(200, "text/json", output);
  });
  
  server.on("/esp/del.php", HTTP_GET, [](AsyncWebServerRequest *request){
    String path = "/" + request -> arg("d");
    if(!SPIFFS.exists(path)) return request -> send(404, "text/plain", "FileNotFound");
    SPIFFS.remove(path);
    request -> send(200, "text/plain", "OK");
    path = String();
  });
  server.on("/esp/rename.php", HTTP_GET, [](AsyncWebServerRequest *request){
    String alt = "/" + request -> arg("old");
    String neu = "/" + request -> arg("new");
    if(!SPIFFS.exists(alt)) return request -> send(404, "text/plain", "FileNotFound");
    SPIFFS.rename(alt, neu);
    request -> send(200, "text/plain", "OK");
    alt = String();
    neu = String();
  });
  //////////////////////////////////////////////////////////////////////////////
  server.on("/doUpdate", HTTP_POST, [](AsyncWebServerRequest *request){},
    [](AsyncWebServerRequest *request, const String& filename,
    size_t index, uint8_t *data, size_t len, bool final){
      handleDoUpdate(request, filename, index, data, len, final);
  });
  //////////////////////////////////////////////////////////////////////////////
  server.onNotFound([](AsyncWebServerRequest *request){
    if(!handleFileRead(request)) request -> send(404);
  });
  //////////////////////////////////////////////////////////////////////////////
  server.begin();
}
