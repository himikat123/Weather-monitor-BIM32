#include <JSON_Decoder.h> // https://github.com/Bodmer/JSON_Decoder
#include <OpenWeather.h> // v0.3.0 https://github.com/Bodmer/OpenWeather/tree/main?tab=readme-ov-file

class Weather {
    #define OPENWEATHERMAP 0
    #define WEATHERBIT 1
    #define OPEN_METEO 2
    #define DAYS 5

    public:
        void update();
        float get_currentTemp();
        float get_currentHum();
        float get_currentPres();
        float get_currentWindSpeed();
        int get_currentWindDir();
        unsigned int get_currentIcon();
        bool get_isDay();
        String get_description();
        time_t get_currentUpdated();

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
        time_t _currentUpdated = -1;
        String _country = "";
        String _city = "";
        String _lon = "";
        String _lat = "";

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

        unsigned int _convertIcon(int code);
        String _openMeteoCode2Description(uint8_t code);
        unsigned int _openMeteoIcon(int code);
        unsigned int _weatherbitIcon(int code);
        void _updateWeatherbitDaily(void);
        void _updateOpenweathermapHourly(void);
        void _updateOpenMeteoDaily(void);
        void _updateOpenMeteoHourly(void);
        void _calculateDaily(void);
};

/**
 * Weather icon code conversion
 */
unsigned int Weather::_convertIcon(int code) {
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

String Weather::_openMeteoCode2Description(uint8_t code) {
    switch(code) {
        case 0: return lang.clearSky();
        case 1: return lang.mainlyClear();
        case 2: return lang.partlyCloudy();
        case 3:	return lang.overcast();
        case 45: return lang.fog();
        case 48: return lang.deposRimeFog();
        case 51: case 53: case 55: return lang.drizzle();
        case 56: case 57: return lang.freezingDrizzle();
        case 61: case 63: case 65: return lang.rain();
        case 66: case 67: return lang.freezingRain();
        case 71: case 73: case 75: return lang.snowFall();
        case 77: return lang.snowGrains();
        case 80: case 81: case 82: return lang.rainShowers();
        case 85: case 86: return lang.snowShowers();
        case 95: return lang.thunderstorm();
        case 96: case 99: return lang.thunderstormWithHail();
        default: return "---";
    }
}

/**
 * Weather update
 */
void Weather::update() {
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

    else if(config.weather_provider() == OPEN_METEO) {
        if(config.weather_citysearch() == 2) {
            if(config.weather_lat() == "" || config.weather_lon() == "") {
                Serial.println("No Coordinates");
                return;
            }
            url = "http://api.open-meteo.com/v1/forecast";
            url += "?latitude=" + config.weather_lat();
            url += "&longitude=" + config.weather_lon();
            url += "&current=temperature_2m,relative_humidity_2m,is_day,weather_code,pressure_msl,wind_speed_10m,wind_direction_10m";
            url += "&wind_speed_unit=ms&timeformat=unixtime&timezone=auto";
        }
    }

    else {
        Serial.println("Wrong weather provider");
        return;
    }
    //Serial.println(url);
    HTTPClient clientWeather;
    clientWeather.begin(url);
    int httpCode = clientWeather.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientWeather.getString();
        //Serial.println(httpData);
        JsonDocument weather;
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
            _lon              = weather["coord"]["lon"].as<String>();
            _lat              = weather["coord"]["lat"].as<String>();
        }

        if(config.weather_provider() == WEATHERBIT) {
            _description      = weather["data"][0]["weather"]["description"].as<String>();
            _currentTemp      = weather["data"][0]["temp"] | 40400.0;
            _currentHum       = weather["data"][0]["rh"] | 40400.0;
            _currentPres      = weather["data"][0]["pres"] | 40400.0;
            _currentWindSpeed = weather["data"][0]["wind_spd"] | -1.0;
            _currentWindDir   = weather["data"][0]["wind_dir"] | -1;
            const char* pod   = weather["data"][0]["pod"] | "";
            _is_day           = (String(pod) == String('d')) ? true : false;
            _country          = weather["data"][0]["country_code"].as<String>();
            _city             = weather["data"][0]["city_name"].as<String>();
            _currentIcon      = _weatherbitIcon(weather["data"][0]["weather"]["code"].as<int>() | 0);
        }

        if(config.weather_provider() == OPEN_METEO) {
            _description      = _openMeteoCode2Description(weather["current"]["weather_code"] | 0);
            _currentTemp      = weather["current"]["temperature_2m"] | 40400.0;
            _currentHum       = weather["current"]["relative_humidity_2m"] | 40400.0;
            _currentPres      = weather["current"]["pressure_msl"] | 40400.0;
            _currentWindSpeed = weather["current"]["wind_speed_10m"] | -1.0;
            _currentWindDir   = weather["current"]["wind_direction_10m"] | -1;
            _is_day           = weather["current"]["is_day"] == 0 ? false : true;
            _currentIcon      = _openMeteoIcon(weather["current"]["weather_code"] | 0);
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
    if(config.weather_provider() == OPEN_METEO) {
        _updateOpenMeteoDaily();
        _updateOpenMeteoHourly();
    }
}

/**
 * Weatherbit icon code conversion
 */
unsigned int Weather::_weatherbitIcon(int code) {
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
 * Open-meteo icon code conversion
 */
unsigned int Weather::_openMeteoIcon(int code) {
    switch(code) {
        case 0: case 1: return 1;
        case 2: return 2;
        case 3:	return 4;
        case 45: case 48: return 50;
        case 51: case 53: case 55: case 56: case 57: return 10;
        case 61: case 63: case 65: case 66: case 67: case 80: case 81: case 82: return 9;
        case 71: case 73: case 75: case 77: case 85: case 86: return 13;
        case 95: case 96: case 99: return 11;
        default: return 1;
    }
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
    //Serial.println(url);
    clientDaily.begin(url);
    int httpCode = clientDaily.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientDaily.getString();
        //Serial.println(httpData);
        JsonDocument forecast;
        DeserializationError errorForecast = deserializeJson(forecast, httpData);
        if(errorForecast) {
            Serial.println("Weatherbit.io: daily forecast deserialization error");
            return;
        }
        for(unsigned int i=0; i<DAYS; i++) {
            _dailyDayTemp[i]   = forecast["data"][i]["high_temp"] | 40400.0;
            _dailyNightTemp[i] = forecast["data"][i]["min_temp"] | 40400.0;
            _dailyWindSpeed[i] = forecast["data"][i]["wind_spd"] | -1.0;
            _dailyIcon[i]      = _weatherbitIcon(forecast["data"][i]["weather"]["code"].as<int>() | 0);
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

    if(_lat.length() and _lon.length()) {
        OW_Weather ow;
        OW_forecast  *forecast;
        forecast = new OW_forecast;
        bool parsed = ow.getForecast(forecast, config.weather_appid(OPENWEATHERMAP), _lat, _lon, "metric", "en", false);

        if(parsed) {
            _hourlyUpdated = now();

            for(unsigned int n=0; n<DAYS*8; n++) {
                _hourlyDate[n] = forecast->dt[n];
                _hourlyIcon[n] = forecast->icon[n].toInt();
                _hourlyTemp[n] = forecast->temp[n];
                _hourlyPres[n] = forecast->pressure[n];
                _hourlyWindSpeed[n] = round(forecast->wind_speed[n]);
                _hourlyWindDir[n] = forecast->wind_deg[n];
                _hourlyPrec[n] = forecast->prec[n];
            }

            Serial.print("Hourly forecast updated successfully at: ");
            Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());

            _calculateDaily();
        }
        else Serial.println("Hourly forecast update error");
        delete forecast;
    }
    else Serial.println("City undefined");
}

/**
 * Update daily forecast from open-meteo.com
 */
void Weather::_updateOpenMeteoDaily() {
    Serial.println(SEPARATOR);
    Serial.println("Open-meteo.com: daily forecast update... ");
  
    String url = "http://api.open-meteo.com/v1/forecast";
    url += "?latitude=" + config.weather_lat();
    url += "&longitude=" + config.weather_lon();
    url += "&daily=weather_code,temperature_2m_max,temperature_2m_min,wind_speed_10m_max";
    url += "&wind_speed_unit=ms&timeformat=unixtime&timezone=auto&forecast_days=4";
    HTTPClient clientDaily;
    //Serial.println(url);
    clientDaily.begin(url);
    int httpCode = clientDaily.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientDaily.getString();
        //Serial.println(httpData);
        JsonDocument forecast;
        DeserializationError errorForecast = deserializeJson(forecast, httpData);
        if(errorForecast) {
            Serial.println("Open-meteo.com: daily forecast deserialization error");
            return;
        }
        for(unsigned int i=0; i<DAYS; i++) {
            _dailyDayTemp[i]   = forecast["daily"]["temperature_2m_max"][i] | 40400.0;
            _dailyNightTemp[i] = forecast["daily"]["temperature_2m_min"][i] | 40400.0;
            _dailyWindSpeed[i] = forecast["daily"]["wind_speed_10m_max"][i] | -1.0;
            _dailyIcon[i]      = _openMeteoIcon(forecast["daily"]["weather_code"][i] | 0);
        }
        httpData = "";
        _dailyUpdated = now();
        Serial.print("Open-meteo.com: daily forecast updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    }
    else Serial.println("Open-meteo.com: daily forecast update error");
    clientDaily.end();
}

/**
 * Update hourly forecast from open-meteo.com
 */
void Weather::_updateOpenMeteoHourly() {
    Serial.println(SEPARATOR);
    Serial.println("Open-meteo.com: hourly forecast update... ");
  
    String url = "http://api.open-meteo.com/v1/forecast";
    url += "?latitude=" + config.weather_lat();
    url += "&longitude=" + config.weather_lon();
    url += "&hourly=temperature_2m,precipitation_probability,weather_code,surface_pressure,wind_speed_10m,wind_direction_10m";
    url += "&wind_speed_unit=ms&timeformat=unixtime&timezone=auto&forecast_days=6";
    HTTPClient clientHourly;
    //Serial.println(url);
    clientHourly.begin(url);
    int httpCode = clientHourly.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientHourly.getString();
        //Serial.println(httpData);
        JsonDocument forecast;
        DeserializationError errorForecast = deserializeJson(forecast, httpData);
        if(errorForecast) {
            Serial.println("Open-meteo.com: hourly forecast deserialization error");
            return;
        }
        uint8_t n = 0;
        for(unsigned int i=0; i<144; i++) {
            time_t time = forecast["hourly"]["time"][i] | 0;
            time_t utc = forecast["utc_offset_seconds"] | 0;
            uint8_t hr = hour(time + utc);
            if(hr == 0 or hr == 3 or hr == 6 or hr == 9 or hr == 12 or hr == 15 or hr == 18 or hr == 21) {
                if((time + utc) >= now()) {
                    _hourlyDate[n] = forecast["hourly"]["time"][i] | 0;
                    _hourlyDate[n] += utc;
                    _hourlyTemp[n] = forecast["hourly"]["temperature_2m"][i] | 40400.0;
                    _hourlyPres[n] = forecast["hourly"]["surface_pressure"][i] | 40400.0;
                    _hourlyWindSpeed[n] = forecast["hourly"]["wind_speed_10m"][i] | -1.0;
                    _hourlyWindDir[n] = forecast["hourly"]["wind_direction_10m"][i] | 0.0;
                    _hourlyPrec[n] = forecast["hourly"]["precipitation_probability"][i] | 0.0;
                    _hourlyIcon[n] = _openMeteoIcon(forecast["hourly"]["weather_code"][i] | 0);
                    if(n<39) n++;
                    else break;
                }
            }
        }
        httpData = "";
        _hourlyUpdated = now();
        Serial.print("Open-meteo.com: hourly forecast updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    }
    else Serial.println("Open-meteo.com: hourly forecast update error");
    clientHourly.end();
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
        if(i == 0) {
            _dailyDayTemp[0] = _currentTemp;;
            _dailyNightTemp[0] = _currentTemp;;
            _dailyIcon[0] = _currentIcon;
            _dailyWindSpeed[0] = _currentWindSpeed;
        }
        for(unsigned int k=0; k<DAYS; k++) {
            if(weekday(_hourlyDate[i]) == wd) {
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
    return _convertIcon(_currentIcon);
}

bool Weather::get_isDay() {
    return _is_day;
}

String Weather::get_description() {
    return _description;
}

time_t Weather::get_currentUpdated() {
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
    return _convertIcon(_dailyIcon[num]);
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