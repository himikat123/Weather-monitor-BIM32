String httpData = "";

void getWeatherNow(void){
  String url;
  url = "http://api.openweathermap.org/data/2.5/weather";
  if(config.citysearch == 0) url += "?q="+String(config.city);
  if(config.citysearch == 1) url += "?id="+String(config.cityid);
  if(config.citysearch == 2) url += "?lat="+String(config.lat) + "&lon="+String(config.lon);
  url += "&units=metric";
  url += "&appid=" + String(config.appid);
  url += "&lang=" + String(config.lang);
  String httpData = weatherNowRequest(url);
  Serial.println(url);
  parseWeatherNow(httpData);
}

String weatherNowRequest(String url){
  String httpData = "";
  HTTPClient client;
  bool find = false;
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
  StaticJsonDocument<1024> root;
  DeserializationError error = deserializeJson(root, httpData);
  if(error) return;
  datas.w_updated = now();
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
  datas.sunrise        = root["sys"]["sunrise"];
  datas.sunset         = root["sys"]["sunset"];
  String country       = root["sys"]["country"];
  country.toCharArray(datas.country, (country.length()) + 1);
  String city          = root["name"];
  city.toCharArray(datas.city, (city.length()) + 1);
  int dayLight = 0;
  if(config.daylight) if(is_summertime()) dayLight = 3600;
  datas.sunrise += config.utc * 3600 + dayLight;
  datas.sunset += config.utc * 3600 + dayLight;
  if(now() > datas.sunrise and now() < datas.sunset) datas.isDay = true;
  else datas.isDay = false;
  httpData = "";
}

OWMfiveForecast    owF5;

void getWeatherDaily(void){
  OWM_fiveForecast *ow_fcast5 = new OWM_fiveForecast[40];
  byte entries = owF5.updateForecast(ow_fcast5, 40, config.appid, datas.country, datas.city, "metric");
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
