class Weather {
  #define OPENWEATHERMAP 0
  #define WEATHERBIT 1
  #define DAYS 5

  public:
    void update();
    bool checkWind(float wind);
    
    float get_currentTemp();
    float get_currentHum();
    float get_currentPres();
    float get_currentWindSpeed();
    int get_currentWindDir();
    unsigned int get_currentIcon();
    bool get_isDay();
    String get_description();
    unsigned int get_currentUpdated();

    float get_dailyDayTemp(unsigned int num);
    float get_dailyNightTemp(unsigned int num);
    float get_dailyWindSpeed(unsigned int num);
    unsigned int get_dailyIcon(unsigned int num);

    unsigned int get_hourlyDate(unsigned int num);
    unsigned int get_hourlyIcon(unsigned int num);
    float get_hourlyTemp(unsigned int num);
    float get_hourlyPres(unsigned int num);
    float get_hourlyWindSpeed(unsigned int num);
    int get_hourlyWindDir(unsigned int num);
    float get_hourlyPrec(unsigned int num); 

  private:
    String _description = "----";
    unsigned int _currentDate = 0;
    float _currentTemp = 40400.0;
    float _currentHum = 40400.0;
    float _currentPres = 40400.0;
    float _currentWindSpeed = -1.0;
    int _currentWindDir = -1;
    unsigned int _currentIcon = 0;
    bool _is_day = false;
    unsigned int _currentUpdated = 0;
    String _country = "";
    String _city = "";

    float _dailyDayTemp[DAYS] = {40400.0, 40400.0, 40400.0, 40400.0, 40400.0};
    float _dailyNightTemp[DAYS] = {40400.0, 40400.0, 40400.0, 40400.0, 40400.0};
    float _dailyWindSpeed[DAYS] = {-1.0, -1.0, -1.0, -1.0, -1.0};
    unsigned int _dailyIcon[DAYS] = {0, 0, 0, 0, 0};
    unsigned int _dailyUpdated = 0;

    unsigned int _hourlyDate[DAYS * 8];
    unsigned int _hourlyIcon[DAYS * 8];
    float _hourlyTemp[DAYS * 8];
    float _hourlyPres[DAYS * 8];
    float _hourlyWindSpeed[DAYS * 8];
    int _hourlyWindDir[DAYS * 8];
    float _hourlyPrec[DAYS * 8];
    unsigned int _hourlyUpdated = 0;

    unsigned int _convert_icon(int code);  
    unsigned int _weatherbit_icon(int code);
    void _updateWeatherbitDaily(void);
    void _updateOpenweathermapHourly(void);
    void _calculateDaily(void);
    void _openweathermapHourly(void);
    void _openweathermapHourlyViaParsingServer(void);
};

/**
 * Check if wind speed is within the normal range
 */
bool Weather::checkWind(float wind) {
  return (wind >= 0.0 and wind <= 100.0);
}

/**
 * Weather icon code conversion
 */
unsigned int Weather::_convert_icon(int code) {
  switch(code) {
    case 1: return 1; break;
    case 2: return 2; break;
    case 3: return 2; break;
    case 4: return 3; break;
    case 9: return 4; break;
    case 10: return 5; break;
    case 11: return 6; break;
    case 13: return 7; break;
    case 50: return 8; break;
    default: return 0; break;
  }
}

/**
 * Weather update
 */
void Weather::update() {
  Serial.println(SEPARATOR);
  Serial.println("Current weather update... ");
  
  String url = "";
  if(config.weather_provider() == OPENWEATHERMAP) {
    if(config.weather_appid(OPENWEATHERMAP) == "") {
      Serial.println("No APPID");
      return;
    }
    url = "http://api.openweathermap.org/data/2.5/weather?appid=" + config.weather_appid(OPENWEATHERMAP);
    if(config.weather_citysearch() == 0) {
      if(config.weather_city() == "") {
        Serial.println("No Cityname");
        return;
      }
      url += "&q=" + config.weather_city();
    }
    if(config.weather_citysearch() == 1) {
      if(config.weather_cityid() == "") {
        Serial.println("No City ID");
        return;
      }
      url += "&id=" + config.weather_cityid();
    }
    if(config.weather_citysearch() == 2) {
      if(config.weather_lat() == "" || config.weather_lon() == "") {
        Serial.println("No Coordinates");
        return;
      }
      url += "&lat=" + config.weather_lat() + "&lon=" + config.weather_lon();
    }
    url += "&units=metric&lang=" + config.lang();
  }
  else if(config.weather_provider() == WEATHERBIT) {
    url = "http://api.weatherbit.io/v2.0/current?key=" + config.weather_appid(WEATHERBIT);
    if(config.weather_citysearch() == 0) {
      if(config.weather_city() == "") {
        Serial.println("No Cityname");
        return;
      }
      url += "&city=" + config.weather_city();
    }
    if(config.weather_citysearch() == 1) {
      if(config.weather_cityid() == "") {
        Serial.println("No City ID");
        return;
      }
      url += "&city_id=" + config.weather_cityid();
    }
    if(config.weather_citysearch() == 2) {
      if(config.weather_lat() == "" || config.weather_lon() == "") {
        Serial.println("No Coordinates");
        return;
      }
      url += "&lat=" + config.weather_lat() + "&lon=" + config.weather_lon();
    }
    url += "&lang=" + config.lang();
  }
  else {
    Serial.println("Wrong weather provider");
    return;
  }
  Serial.println(url);
  HTTPClient clientWeather;
  clientWeather.begin(url);
  int httpCode = clientWeather.GET();
  if(httpCode == HTTP_CODE_OK) {
    String httpData = clientWeather.getString();
    Serial.println(httpData);
    StaticJsonDocument<8192> weather;
    DeserializationError errorWeather = deserializeJson(weather, httpData);
    if(errorWeather) {
      Serial.println("Current weather deserialization error");
      return;
    }
    if(config.weather_provider() == OPENWEATHERMAP) {
      _description      = weather["weather"][0]["description"].as<String>();
      _currentTemp      = weather["main"]["temp"] | 40400.0;
      _currentHum       = weather["main"]["humidity"] | 40400.0;
      _currentPres      = weather["main"]["pressure"] | 40400.0;
      _currentWindSpeed = weather["wind"]["speed"] | -1.0;
      _currentWindDir   = weather["wind"]["deg"] | -1;
      _currentIcon      = atoi(weather["weather"][0]["icon"]);
      String pod        = weather["weather"][0]["icon"] | "";
      _is_day           = (pod.substring(2) == "d") ? true : false;
      _country          = weather["sys"]["country"].as<String>();
      _city             = weather["name"].as<String>();
      _currentDate      = weather["dt"];
    }
    if(config.weather_provider() == WEATHERBIT) {
      _description      = weather["data"][0]["weather"]["description"].as<String>();
      _currentTemp      = weather["data"][0]["temp"] | 40400.0;
      _currentHum       = weather["data"][0]["rh"] | 40400.0;
      _currentPres      = weather["data"][0]["pres"] | 40400.0;
      _currentWindSpeed = weather["data"][0]["wind_spd"] | -1.0;
      _currentWindDir   = weather["data"][0]["wind_dir"] | -1;
      _currentIcon      = _weatherbit_icon(weather["data"][0]["weather"]["code"].as<int>() | 0);
      const char* pod   = weather["data"][0]["pod"] | "";
      _is_day           = (String(pod) == String('d')) ? true : false;
      _country          = weather["data"][0]["country_code"].as<String>();
      _city             = weather["data"][0]["city_name"].as<String>();
    }
    httpData = "";
    _currentUpdated = now();
    Serial.print("Current weather updated successfully at: ");
    Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
  }
  else Serial.println("Current weather update error");
  clientWeather.end();

  if(config.weather_provider() == WEATHERBIT) _updateWeatherbitDaily();
  if(config.weather_provider() == OPENWEATHERMAP) _updateOpenweathermapHourly();
}

/**
 * Weatherbit icon code conversion
 */
unsigned int Weather::_weatherbit_icon(int code) {
  unsigned int icon = 4;
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

/**
 * Update daily forecast from weatherbit.io
 */
void Weather::_updateWeatherbitDaily(void) {
  Serial.println(SEPARATOR);
  Serial.println("Weatherbit.io: daily forecast update... ");
  
  String url = "http://api.weatherbit.io/v2.0/forecast/daily?days=4";
  if(config.weather_citysearch() == 0) url += "&city=" + config.weather_city();
  if(config.weather_citysearch() == 1) url += "&city_id=" + config.weather_cityid();
  if(config.weather_citysearch() == 2) url += "&lat=" + config.weather_lat() + "&lon=" + config.weather_lon();
  url += "&key=" + config.weather_appid(WEATHERBIT);
  HTTPClient clientDaily;
  Serial.println(url);
  clientDaily.begin(url);
  int httpCode = clientDaily.GET();
  if(httpCode == HTTP_CODE_OK) {
    String httpData = clientDaily.getString();
    Serial.println(httpData);
    DynamicJsonDocument forecast(8192);
    DeserializationError errorForecast = deserializeJson(forecast, httpData);
    if(errorForecast) {
      Serial.println("Weatherbit.io: daily forecast deserialization error");
      return;
    }
    for(unsigned int i=0; i<DAYS; i++) {
      _dailyDayTemp[i]   = forecast["data"][i]["high_temp"] | 40400.0;
      _dailyNightTemp[i] = forecast["data"][i]["min_temp"] | 40400.0;
      _dailyIcon[i]      = _weatherbit_icon(forecast["data"][i]["weather"]["code"].as<int>() | 0);
      _dailyWindSpeed[i] = forecast["data"][i]["wind_spd"] | -1.0;
    }
    httpData = "";
    _dailyUpdated = now();
    Serial.print("Weatherbit.io: daily forecast updated successfully at: ");
    Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
  }
  else Serial.println("Weatherbit.io: daily forecast update error");
  clientDaily.end();
}

/**
 * Update 3 hourly forecast from openweathermap.org
 */
void Weather::_updateOpenweathermapHourly(void) {
  Serial.println(SEPARATOR);
  Serial.println("Hourly forecast update... ");

  if(_country != "" && _city != "") {
    if(config.weather_parsingServer() == "") {
      _openweathermapHourly();
    }
    else _openweathermapHourlyViaParsingServer();
  }
}

/**
 * Update 3 hourly forecast from openweathermap.org via parsing server
 */
void Weather::_openweathermapHourlyViaParsingServer(void) {
  Serial.println("Update via parsing server... ");
  String location = "";
  if(config.weather_citysearch() == 0) {
    if(config.weather_city() == "") return;
    location = "?q=" + config.weather_city();
  }
  if(config.weather_citysearch() == 1) {
    if(config.weather_cityid() == "") return;
    location = "?id=" + config.weather_cityid();
  }
  if(config.weather_citysearch() == 2) {
    if(config.weather_lat() == "" || config.weather_lon() == "") return;
    location = "?lat=" + config.weather_lat() + "&lon=" + config.weather_lon();
  }
  String url = config.weather_parsingServer();
  url += location;
  url += "&provider=" + String(config.weather_provider());
  url += "&appid=" + config.weather_appid(config.weather_provider());
  url += "&mac=" + WiFi.macAddress();
  HTTPClient clientHourly;
  Serial.println(url);
  clientHourly.begin(url);
  int httpCode = clientHourly.GET();
  if(httpCode == HTTP_CODE_OK) {
    String httpData = clientHourly.getString();
    Serial.println(httpData);
    DynamicJsonDocument hourly(8192);
    DeserializationError errorHourly = deserializeJson(hourly, httpData);
    if(errorHourly) {
      Serial.println("Hourly forecast deserialization error");
      _openweathermapHourly();
      return;
    }
    for(unsigned int i=0; i<DAYS*8; i++) {
      _hourlyDate[i] = hourly["date"][i] | 0;
      _hourlyIcon[i] = hourly["icon"][i] | 0;
      _hourlyTemp[i] = hourly["temp"][i] | 40400.0;
      _hourlyPres[i] = hourly["pres"][i] | 40400.0;
      _hourlyWindSpeed[i] = hourly["wSpeed"][i] | -1.0;
      _hourlyWindDir[i] = hourly["wDir"][i] | -1;
      _hourlyPrec[i] = hourly["prec"][i] | -1.0;
    }
    httpData = "";
    _hourlyUpdated = now();
    Serial.print("Hourly forecast updated successfully at: ");
    Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    _calculateDaily();
  }
  else {
    Serial.println("Hourly forecast update error");
    _openweathermapHourly();
  }
  clientHourly.end();
}

/**
 * Update 3 hourly forecast direct from openweathermap.org
 */
void Weather::_openweathermapHourly(void) {
  Serial.println("Update from openweathermap server... ");
  OWMfiveForecast owF5;
  OWM_fiveForecast *ow_fcast5 = new OWM_fiveForecast[40];
  String location = "";
  if(config.weather_citysearch() == 0) {
    if(config.weather_city() == "") return;
    location = "q=" + config.weather_city();
  }
  if(config.weather_citysearch() == 1) {
    if(config.weather_cityid() == "") return;
    location = "id=" + config.weather_cityid();
  }
  if(config.weather_citysearch() == 2) {
    if(config.weather_lat() == "" || config.weather_lon() == "") return;
    location = "lat=" + config.weather_lat() + "&lon=" + config.weather_lon();
  }
  unsigned int entries = owF5.updateForecast(ow_fcast5, 40, config.weather_appid(config.weather_provider()), location, "metric");
  for(unsigned int i=0; i<=entries; i++) { 
    _hourlyDate[i] = ow_fcast5[i].dt.toInt();
    _hourlyIcon[i] = ow_fcast5[i].icon.toInt();
    _hourlyTemp[i] = (ow_fcast5[i].t_min.toFloat() + ow_fcast5[i].t_max.toFloat()) / 2;
    _hourlyPres[i] = ow_fcast5[i].pressure.toInt();
    _hourlyWindSpeed[i] = round(ow_fcast5[i].w_speed.toFloat());
    _hourlyWindDir[i] = ow_fcast5[i].w_deg.toInt();
    _hourlyPrec[i] = (ow_fcast5[i].cond == "rain" or ow_fcast5[i].cond == "snow") ? ow_fcast5[i].cond_value.toInt() : 0;
  }
  if(entries) {
    _hourlyUpdated = now();
    Serial.print("Hourly forecast updated successfully at: ");
    Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    _calculateDaily();
  }
  else Serial.println("Hourly forecast update error");
}

/**
 * Calculate daily from hourly forecast
 */
void Weather::_calculateDaily(void) {
  Serial.print("Calculate daily forecast... ");
  for(unsigned int i=0; i<DAYS; i++) {
    _dailyDayTemp[i] = -40400.0;
    _dailyNightTemp[i] = 40400.0;
    _dailyWindSpeed[i] = -1.0;
    _dailyIcon[i] = 0;
  }
  for(unsigned int i=0; i<DAYS*8; i++) {
    unsigned int wd = weekday(_currentDate);
    unsigned int hr = hour(_hourlyDate[i]);
    if(i == 0){
      _dailyDayTemp[0] = _currentTemp;;
      _dailyNightTemp[0] = _currentTemp;;
      _dailyIcon[0] = _currentIcon;
      _dailyWindSpeed[0] = _currentWindSpeed;
    }
    for(unsigned int k=0; k<DAYS; k++) {
      if(weekday(_hourlyDate[i]) == wd){
        if(_dailyDayTemp[k] < _hourlyTemp[i]) _dailyDayTemp[k] = _hourlyTemp[i];
        if(_dailyNightTemp[k] > _hourlyTemp[i]) _dailyNightTemp[k] = _hourlyTemp[i];
        if(_dailyIcon[k] < _hourlyIcon[i]) _dailyIcon[k] = _hourlyIcon[i];
        if(_dailyWindSpeed[k] < _hourlyWindSpeed[i]) _dailyWindSpeed[k] = _hourlyWindSpeed[i];
      }
      if(++wd > 7) wd = 1;
    }
  }
  Serial.println("done");
}

/**
 * Getters 
 */
float Weather::get_currentTemp() {
  return _currentTemp;
}

float Weather::get_currentHum() {
  return _currentHum;
}

float Weather::get_currentPres() {
  return _currentPres;
}

float Weather::get_currentWindSpeed() {
  return _currentWindSpeed;
}

int Weather::get_currentWindDir() {
  return _currentWindDir;
}

unsigned int Weather::get_currentIcon() {
  return _convert_icon(_currentIcon);
}

bool Weather::get_isDay() {
  return _is_day;
}

String Weather::get_description() {
  return _description;
}

unsigned int Weather::get_currentUpdated() {
  return _currentUpdated;
}

float Weather::get_dailyDayTemp(unsigned int num) {
  if(num >= DAYS) return 40400.0;
  return _dailyDayTemp[num];
}

float Weather::get_dailyNightTemp(unsigned int num) {
  if(num >= DAYS) return 40400.0;
  return _dailyNightTemp[num];
}

float Weather::get_dailyWindSpeed(unsigned int num) {
  if(num >= DAYS) return -1.0;
  return _dailyWindSpeed[num];
}

unsigned int Weather::get_dailyIcon(unsigned int num) {
  if(num >= DAYS) return 0;
  return _convert_icon(_dailyIcon[num]);
}

unsigned int Weather::get_hourlyDate(unsigned int num) {
  if(num >= DAYS * 8) return 0;
  return _hourlyDate[num];
}

unsigned int Weather::get_hourlyIcon(unsigned int num) {
  if(num >= DAYS * 8) return 0;
  return _hourlyIcon[num];
}

float Weather::get_hourlyTemp(unsigned int num) {
  if(num >= DAYS * 8) return 40400.0;
  return _hourlyTemp[num];
}

float Weather::get_hourlyPres(unsigned int num) {
  if(num >= DAYS * 8) return 40400.0;
  return _hourlyPres[num];
}

float Weather::get_hourlyWindSpeed(unsigned int num) {
  if(num >= DAYS * 8) return -1.0;
  return _hourlyWindSpeed[num];
}

int Weather::get_hourlyWindDir(unsigned int num) {
  if(num >= DAYS * 8) return -1;
  return _hourlyWindDir[num];
}

float Weather::get_hourlyPrec(unsigned int num) {
  if(num >= DAYS * 8) return -1.0;
  return _hourlyPrec[num];
}
