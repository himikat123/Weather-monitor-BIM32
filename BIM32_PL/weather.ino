String httpData = "";

uint8_t weatherbit_icon(int code){
  uint8_t icon = 4;
  if(code >= 200 and code < 300) icon = 11;
  if(code >= 300 and code < 400) icon = 9;
  if(code >= 500 and code < 600) icon = 10;
  if(code >= 600 and code < 700) icon = 13;
  if(code >= 700 and code < 800) icon = 50;
  if(code == 800) icon = 1;
  if(code == 801 or code == 802) icon = 2;
  if(code == 803) icon = 3;
  if(code >= 804) icon = 4;
  return icon;
}

void getWeatherNow(void){
  String url;
  if(config.provider == 0){    
    url = "http://api.openweathermap.org/data/2.5/weather";
    if(config.citysearch == 0) url += "?q=" + String(config.city);
    if(config.citysearch == 1) url += "?lat=" + String(config.lat) + "&lon=" + String(config.lon);
    if(config.citysearch == 2) url += "?id=" + String(config.cityid);
    url += "&units=metric";
    url += "&appid=" + String(config.appid);
    url += "&lang=" + String(config.lang);
  }
  if(config.provider == 1){
    url = "http://api.weatherbit.io/v2.0/current?key=";
    url += String(config.appkey);
    if(config.citysearch == 0) url += "&city=" + String(config.city);
    if(config.citysearch == 1) url += "&lat=" + String(config.lat) + "&lon=" + String(config.lon);
    if(config.citysearch == 2) url += "&city_id=" + String(config.cityid);
    url += "&lang=" + String(config.lang);
  }
  Serial.println(url);
  parseWeatherNow(weatherRequest(url));
}

String weatherRequest(String url){
  String httpData = "";
  HTTPClient client;
  client.begin(url);
  int httpCode = client.GET();
  if(httpCode > 0){
    if(httpCode == HTTP_CODE_OK){
      httpData = client.getString();
    }
  }
  client.end();
  return httpData;
}

void parseWeatherNow(String httpData){
  Serial.println(httpData);
  StaticJsonDocument<2048> root;
  DeserializationError error = deserializeJson(root, httpData);
  Serial.print("deserialization=");
  Serial.println(error ? "error" : "OK");
  if(error) return;
  if(config.provider == 0){
    String descript      = root["weather"][0]["description"];
    descript.toCharArray(datas.description, (descript.length()) + 1);
    datas.icon[0]        = atoi(root["weather"][0]["icon"]);
    datas.temp_web       = root["main"]["temp"];
    datas.temp_web       = round(datas.temp_web * 10) / 10;
    datas.hum_web        = root["main"]["humidity"];
    datas.pres_web       = root["main"]["pressure"];
    datas.pres_web      *= 0.75;
    datas.w_speed[0]     = root["wind"]["speed"];
    datas.w_dir[0]       = root["wind"]["deg"];
    //datas.sunrise        = root["sys"]["sunrise"];
    //datas.sunset         = root["sys"]["sunset"];
    String country       = root["sys"]["country"];
    country.toCharArray(datas.country, (country.length()) + 1);
    String city          = root["name"];
    city.toCharArray(datas.city, (city.length()) + 1);
    //int dayLight = 0;
    //if(config.daylight) if(is_summertime()) dayLight = 3600;
    //datas.sunrise += config.utc * 3600 + dayLight;
    //datas.sunset += config.utc * 3600 + dayLight;
    String pod = root["weather"][0]["icon"];
    Serial.print("icon=");
    Serial.println(pod);
    pod = pod.substring(2);
    Serial.print("pod=");
    Serial.println(pod);
    if(pod == "d") datas.isDay = true;
    //if(now() > datas.sunrise and now() < datas.sunset) datas.isDay = true;
    else datas.isDay = false;
  }
  if(config.provider == 1){
    const char* descript = root["data"][0]["weather"]["description"];
    sprintf(datas.description, "%s", descript);
    datas.icon[0]        = weatherbit_icon(root["data"][0]["weather"]["code"].as<int>());
    datas.temp_web       = root["data"][0]["temp"];
    datas.temp_web       = round(datas.temp_web * 10) / 10;
    datas.hum_web        = root["data"][0]["rh"];
    datas.pres_web       = root["data"][0]["pres"];
    datas.pres_web      *= 0.75;
    datas.w_speed[0]     = root["data"][0]["wind_spd"];
    datas.w_dir[0]       = root["data"][0]["wind_dir"];
    const char* pod      = root["data"][0]["pod"];
    if(String(pod) == String('d')) datas.isDay = true;
    else datas.isDay = false;
    //String sunrise = root["data"][0]["sunrise"];
    //String sunset  = root["data"][0]["sunset"];
    //TimeElements tm;
    //char buf[10];
    //sunrise.toCharArray(buf, 10);
    //tm.Hour = atoi(strtok(buf, ":"));
    //tm.Hour += config.utc;
    //tm.Minute = atoi(strtok(NULL, ":"));
    //tm.Wday = 0; tm.Second = 0; tm.Day = 1; tm.Month = 1; tm.Year = 0;
    //datas.sunrise = makeTime(tm);
    //sunset.toCharArray(buf, 10);
    //tm.Hour = atoi(strtok(buf, ":"));
    //tm.Hour += config.utc;
    //tm.Minute = atoi(strtok(NULL, ":"));
    //datas.sunset = makeTime(tm);
    String country      = root["data"][0]["country_code"];
    country.toCharArray(datas.country, (country.length()) + 1);
    String city         = root["data"][0]["city_name"];
    city.toCharArray(datas.city, (city.length()) + 1);
  }
  httpData = "";
}

OWMfiveForecast    owF5;

void getWeatherDaily(void){
  if(config.provider == 0){
    OWM_fiveForecast *ow_fcast5 = new OWM_fiveForecast[40];
    byte entries = owF5.updateForecast(ow_fcast5, 40, config.appid, datas.country, datas.city, "metric");
    datas.w_dir[1] = 0;
    datas.w_dir[2] = 0;
    datas.w_dir[3] = 0;
    datas.w_dir[4] = 0;
    for(byte i = 0; i <= entries; ++i){ 
      datas.date3hourly[i] = ow_fcast5[i].dt.toInt();
      datas.icon3hourly[i] = ow_fcast5[i].icon.toInt();
      datas.temp3hourly[i] = (ow_fcast5[i].t_min.toFloat() + ow_fcast5[i].t_max.toFloat()) / 2;
      datas.pres3hourly[i] = 0.75 * ow_fcast5[i].pressure.toInt();
      datas.wspd3hourly[i] = round(ow_fcast5[i].w_speed.toFloat());
      datas.wdeg3hourly[i] = ow_fcast5[i].w_deg.toInt();
      datas.prec3hourly[i] = (ow_fcast5[i].cond=="rain" or ow_fcast5[i].cond=="snow") ? ow_fcast5[i].cond_value.toInt() : 0;
      uint8_t wd = weekday();
      uint8_t hr = hour(datas.date3hourly[i]);
      if(i == 0){
        datas.temp[2] = datas.temp_web;
        datas.temp[3] = datas.temp_web;
        datas.icon[1] = datas.icon[0];
        datas.w_speed[1] = datas.w_speed[0];
        datas.w_dir[1] = datas.w_dir[0];
      }
      if(weekday(datas.date3hourly[i]) == wd){
        if(hr == 0 or datas.temp[2] < datas.temp3hourly[i]) datas.temp[2] = datas.temp3hourly[i];
        if(hr == 0 or datas.temp[3] > datas.temp3hourly[i]) datas.temp[3] = datas.temp3hourly[i];
        if(hr == 0 or datas.icon[1] < datas.icon3hourly[i]) datas.icon[1] = datas.icon3hourly[i];
        if(hr == 0 or datas.w_speed[1] < datas.wspd3hourly[i]) datas.w_speed[1] = datas.wspd3hourly[i];
        if(hr == 0) datas.w_dir[1] = datas.wdeg3hourly[i];
        else datas.w_dir[1] = (datas.w_dir[1] + datas.wdeg3hourly[i]) / 2;
      }
      if(++wd > 7) wd = 1;
      if(weekday(datas.date3hourly[i]) == wd){
        if(hr == 0 or datas.temp[4] < datas.temp3hourly[i]) datas.temp[4] = datas.temp3hourly[i];
        if(hr == 0 or datas.temp[5] > datas.temp3hourly[i]) datas.temp[5] = datas.temp3hourly[i];
        if(hr == 0 or datas.icon[2] < datas.icon3hourly[i]) datas.icon[2] = datas.icon3hourly[i];
        if(hr == 0 or datas.w_speed[2] < datas.wspd3hourly[i]) datas.w_speed[2] = datas.wspd3hourly[i];
        if(hr == 0) datas.w_dir[2] = datas.wdeg3hourly[i];
        else datas.w_dir[2] = (datas.w_dir[2] + datas.wdeg3hourly[i]) / 2;
      }
      if(++wd > 7) wd = 1;
      if(weekday(datas.date3hourly[i]) == wd){
        if(hr == 0 or datas.temp[6] < datas.temp3hourly[i]) datas.temp[6] = datas.temp3hourly[i];
        if(hr == 0 or datas.temp[7] > datas.temp3hourly[i]) datas.temp[7] = datas.temp3hourly[i];
        if(hr == 0 or datas.icon[3] < datas.icon3hourly[i]) datas.icon[3] = datas.icon3hourly[i];
        if(hr == 0 or datas.w_speed[3] < datas.wspd3hourly[i]) datas.w_speed[3] = datas.wspd3hourly[i];
        if(hr == 0) datas.w_dir[3] = datas.wdeg3hourly[i];
        else datas.w_dir[3] = (datas.w_dir[3] + datas.wdeg3hourly[i]) / 2;
      }
      if(++wd > 7) wd = 1;
      if(weekday(datas.date3hourly[i]) == wd){
        if(hr == 0 or datas.temp[8] < datas.temp3hourly[i]) datas.temp[8] = datas.temp3hourly[i];
        if(hr == 0 or datas.temp[9] > datas.temp3hourly[i]) datas.temp[9] = datas.temp3hourly[i];
        if(hr == 0 or datas.icon[4] < datas.icon3hourly[i]) datas.icon[4] = datas.icon3hourly[i];
        if(hr == 0 or datas.w_speed[4] < datas.wspd3hourly[i]) datas.w_speed[4] = datas.wspd3hourly[i];
        if(hr == 0) datas.w_dir[4] = datas.wdeg3hourly[i];
        else datas.w_dir[4] = (datas.w_dir[4] + datas.wdeg3hourly[i]) / 2;
      }
    }
    delete[] ow_fcast5;
  }
  if(config.provider == 1){
    String url = "http://api.weatherbit.io/v2.0/forecast/daily?key=";
    url += String(config.appkey);
    if(config.citysearch == 0) url += "&city=" + String(config.city);
    if(config.citysearch == 1) url += "&lat=" + String(config.lat) + "&lon=" + String(config.lon);
    if(config.citysearch == 2) url += "&city_id=" + String(config.cityid);
    url += "&days=4";
    Serial.println(url);
    parseWeatherDaily(weatherRequest(url));
  }
  datas.w_updated = now();
}

void parseWeatherDaily(String httpData){
  Serial.println(httpData);
  StaticJsonDocument<8192> root;
  DeserializationError error = deserializeJson(root, httpData);
  Serial.print("deserialization=");
  Serial.println(error ? "error" : "OK");
  if(error) return;
  datas.temp[2]    = root["data"][0]["high_temp"];
  datas.temp[3]    = root["data"][0]["min_temp"];
  datas.icon[1]    = weatherbit_icon(root["data"][0]["weather"]["code"].as<int>());
  datas.w_speed[1] = root["data"][0]["wind_spd"];
  datas.w_dir[1]   = root["data"][0]["wind_dir"];
  datas.temp[4]    = root["data"][1]["high_temp"];
  datas.temp[5]    = root["data"][1]["min_temp"];
  datas.icon[2]    = weatherbit_icon(root["data"][1]["weather"]["code"].as<int>());
  datas.w_speed[2] = root["data"][1]["wind_spd"];
  datas.w_dir[2]   = root["data"][1]["wind_dir"];
  datas.temp[6]    = root["data"][2]["high_temp"];
  datas.temp[7]    = root["data"][2]["min_temp"];
  datas.icon[3]    = weatherbit_icon(root["data"][2]["weather"]["code"].as<int>());
  datas.w_speed[3] = root["data"][2]["wind_spd"];
  datas.w_dir[3]   = root["data"][2]["wind_dir"];
  datas.temp[8]    = root["data"][3]["high_temp"];
  datas.temp[9]    = root["data"][3]["min_temp"];
  datas.icon[4]    = weatherbit_icon(root["data"][3]["weather"]["code"].as<int>());
  datas.w_speed[4] = root["data"][3]["wind_spd"];
  datas.w_dir[4]   = root["data"][3]["wind_dir"];
  httpData = "";
}
