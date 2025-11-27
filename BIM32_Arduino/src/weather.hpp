#include <JSON_Decoder.h> // https://github.com/Bodmer/JSON_Decoder
#include <OpenWeather.h> // v0.3.0 https://github.com/Bodmer/OpenWeather/tree/main?tab=readme-ov-file

class Weather {
    #define OPENWEATHERMAP 0
    #define WEATHERBIT 1
    #define OPEN_METEO 2
    #define DAYS 5

    public:
        void update();
        float get_currentTemp(bool corr);
        float get_currentHum(bool corr);
        float get_currentPres(bool corr);
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
        //unsigned int _currentDate = 0;
        //float _currentWindSpeed = -1.0;
        //String _country = "";
        //String _city = "";
        float _lon = 0.0;
        float _lat = 0.0;

        // float _dailyDayTemp[DAYS] = {40400.0, 40400.0, 40400.0, 40400.0, 40400.0};
        // float _dailyNightTemp[DAYS] = {40400.0, 40400.0, 40400.0, 40400.0, 40400.0};
        // float _dailyWindSpeed[DAYS] = {-1.0, -1.0, -1.0, -1.0, -1.0};
        // unsigned int _dailyIcon[DAYS] = {0, 0, 0, 0, 0};
        // unsigned int _dailyUpdated = 0;

        // unsigned int _hourlyDate[DAYS * 8];
        // unsigned int _hourlyIcon[DAYS * 8];
        // float _hourlyTemp[DAYS * 8];
        // float _hourlyPres[DAYS * 8];
        // float _hourlyWindSpeed[DAYS * 8];
        // int _hourlyWindDir[DAYS * 8];
        // float _hourlyPrec[DAYS * 8];
        // unsigned int _hourlyUpdated = 0;

        unsigned int _convertIcon(int code);
        const char* _openMeteoCode2Description(uint8_t code);
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

const char* Weather::_openMeteoCode2Description(uint8_t code) {
    switch(code) {
        case 0: return lang.weatherDescription(0);
        case 1: return lang.weatherDescription(1);
        case 2: return lang.weatherDescription(2);
        case 3:	return lang.weatherDescription(3);
        case 45: return lang.weatherDescription(4);
        case 48: return lang.weatherDescription(5);
        case 51: case 53: case 55: return lang.weatherDescription(6);
        case 56: case 57: return lang.weatherDescription(7);
        case 61: case 63: case 65: return lang.weatherDescription(8);
        case 66: case 67: return lang.weatherDescription(9);
        case 71: case 73: case 75: return lang.weatherDescription(10);
        case 77: return lang.weatherDescription(11);
        case 80: case 81: case 82: return lang.weatherDescription(12);
        case 85: case 86: return lang.weatherDescription(13);
        case 95: return lang.weatherDescription(14);
        case 96: case 99: return lang.weatherDescription(15);
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
        if(global.debugWether) {
            Serial.println("OPENWEATHERMAP");
            Serial.println(url);
        }
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
        if(global.debugWether) {
            Serial.println("WEATHERBIT");
            Serial.println(url);
        }
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
        if(global.debugWether) {
            Serial.println("OPEN_METEO");
            Serial.println(url);
        }
    }

    else {
        Serial.println("Wrong weather provider");
        return;
    }
    HTTPClient clientWeather;
    clientWeather.begin(url);
    int httpCode = clientWeather.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientWeather.getString();
        JsonDocument weather;
        DeserializationError errorWeather = deserializeJson(weather, httpData);
        if(global.debugWether) {
            Serial.println(httpData);
        }
        if(errorWeather) {
            Serial.println("Current weather deserialization error");
            return;
        }

        if(config.weather_provider() == OPENWEATHERMAP) {
            strlcpy(state.weather.descript, weather["weather"][0]["description"] | "", sizeof(state.weather.descript));
            state.weather.temp       = weather["main"]["temp"] | 40400.0;
            state.weather.hum        = weather["main"]["humidity"] | 40400.0;
            state.weather.pres       = weather["main"]["pressure"] | 40400.0;
            state.weather.wind.speed = weather["wind"]["speed"] | -1.0;
            state.weather.wind.dir   = weather["wind"]["deg"] | -1;
            state.weather.icon       = atoi(weather["weather"][0]["icon"] | "0");
            String pod               = weather["weather"][0]["icon"] | "";
            state.weather.isDay      = (pod.substring(2) == "d") ? true : false;
            //_country          = weather["sys"]["country"].as<String>();
            //_city             = weather["name"].as<String>();
            state.weather.time       = weather["dt"] | 0;
            _lon                     = weather["coord"]["lon"] | 0.0;
            _lat                     = weather["coord"]["lat"] | 0.0;
        }

        if(config.weather_provider() == WEATHERBIT) {
            strlcpy(state.weather.descript, weather["data"][0]["weather"]["description"] | "", sizeof(state.weather.descript));
            state.weather.temp       = weather["data"][0]["temp"] | 40400.0;
            state.weather.hum        = weather["data"][0]["rh"] | 40400.0;
            state.weather.pres       = weather["data"][0]["pres"] | 40400.0;
            state.weather.wind.speed = weather["data"][0]["wind_spd"] | -1.0;
            state.weather.wind.dir   = weather["data"][0]["wind_dir"] | -1;
            const char* pod          = weather["data"][0]["pod"] | "";
            state.weather.isDay      = (String(pod) == String('d')) ? true : false;
            //_country          = weather["data"][0]["country_code"].as<String>();
            //_city             = weather["data"][0]["city_name"].as<String>();
            state.weather.icon       = _weatherbitIcon(weather["data"][0]["weather"]["code"].as<int>() | 0);
        }

        if(config.weather_provider() == OPEN_METEO) {
            String descr        = _openMeteoCode2Description(weather["current"]["weather_code"] | 0);
            strlcpy(state.weather.descript, descr, sizeof(state.weather.descript));
            state.weather.temp       = weather["current"]["temperature_2m"] | 40400.0;
            state.weather.hum        = weather["current"]["relative_humidity_2m"] | 40400.0;
            state.weather.pres       = weather["current"]["pressure_msl"] | 40400.0;
            state.weather.wind.speed = weather["current"]["wind_speed_10m"] | -1.0;
            state.weather.wind.dir   = weather["current"]["wind_direction_10m"] | -1;
            state.weather.isDay      = weather["current"]["is_day"] == 0 ? false : true;
            state.weather.icon       = _openMeteoIcon(weather["current"]["weather_code"] | 0);
        }

        httpData = "";
        state.weather.time = now();
        Serial.print("Current weather updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    }
    else Serial.println("Current weather update error");
    clientWeather.end();

    #if !defined(BIM32_CYD)
        if(config.display_type(0) == 1) {
    #endif
            if(config.weather_provider() == WEATHERBIT) _updateWeatherbitDaily();
            if(config.weather_provider() == OPENWEATHERMAP) _updateOpenweathermapHourly();
            if(config.weather_provider() == OPEN_METEO) {
                _updateOpenMeteoDaily();
                if(config.display_model(0) == 0 || config.display_model(0) == 2) _updateOpenMeteoHourly();
            }
    #if !defined(BIM32_CYD)
        }
    #endif
    state.weather.updated = true;
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
    if(global.debugWether) {
        Serial.println(url);
    }
    clientDaily.begin(url);
    int httpCode = clientDaily.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientDaily.getString();
        if(global.debugWether) {
            Serial.println(httpData);
        }
        JsonDocument forecast;
        DeserializationError errorForecast = deserializeJson(forecast, httpData);
        if(errorForecast) {
            Serial.println("Weatherbit.io: daily forecast deserialization error");
            return;
        }
        for(unsigned int i=0; i<DAYS; i++) {
            state.weather.daily.tMax[i] = forecast["data"][i]["high_temp"] | 40400.0;
            state.weather.daily.tMin[i] = forecast["data"][i]["min_temp"] | 40400.0;
            state.weather.daily.wind[i] = forecast["data"][i]["wind_spd"] | -1.0;
            state.weather.daily.icon[i] = _weatherbitIcon(forecast["data"][i]["weather"]["code"].as<int>() | 0);
        }
        httpData = "";
        state.weather.daily.time = now();
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

    OW_Weather ow;
    OW_forecast  *forecast;
    forecast = new OW_forecast;
    bool parsed = ow.getForecast(
        forecast, 
        config.weather_appid(OPENWEATHERMAP), 
        String(_lat), 
        String(_lon), 
        "metric", "en", false
    );

    if(parsed) {
        state.weather.hourly.time = now();

        for(unsigned int n=0; n<DAYS*8; n++) {
            state.weather.hourly.date[n] = forecast->dt[n];
            state.weather.hourly.icon[n] = forecast->icon[n].toInt();
            state.weather.hourly.temp[n] = forecast->temp[n];
            state.weather.hourly.pres[n] = forecast->pressure[n];
            state.weather.hourly.windSpeed[n] = round(forecast->wind_speed[n]);
            state.weather.hourly.windDir[n] = forecast->wind_deg[n];
            state.weather.hourly.prec[n] = forecast->prec[n];
        }

        Serial.print("Hourly forecast updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());

        _calculateDaily();
    }
    else Serial.println("Hourly forecast update error");
    delete forecast;
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
    if(global.debugWether) {
        Serial.println(url);
    }
    clientDaily.begin(url);
    int httpCode = clientDaily.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientDaily.getString();
        if(global.debugWether) {
            Serial.println(httpData);
        }
        JsonDocument forecast;
        DeserializationError errorForecast = deserializeJson(forecast, httpData);
        if(errorForecast) {
            Serial.println("Open-meteo.com: daily forecast deserialization error");
            return;
        }
        for(unsigned int i=0; i<DAYS; i++) {
            state.weather.daily.tMax[i] = forecast["daily"]["temperature_2m_max"][i] | 40400.0;
            state.weather.daily.tMin[i] = forecast["daily"]["temperature_2m_min"][i] | 40400.0;
            state.weather.daily.wind[i] = forecast["daily"]["wind_speed_10m_max"][i] | -1.0;
            state.weather.daily.icon[i] = _openMeteoIcon(forecast["daily"]["weather_code"][i] | 0);
        }
        httpData = "";
        state.weather.daily.time = now();
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
    if(global.debugWether) {
        Serial.println(url);
    }
    clientHourly.begin(url);
    int httpCode = clientHourly.GET();
    if(httpCode == HTTP_CODE_OK) {
        String httpData = clientHourly.getString();
        if(global.debugWether) {
            Serial.println(httpData);
        }
        JsonDocument filter[7];
        filter[0]["utc_offset_seconds"] = true;
        filter[0]["hourly"]["time"] = true;
        filter[1]["hourly"]["temperature_2m"] = true;
        filter[2]["hourly"]["precipitation_probability"] = true;
        filter[3]["hourly"]["weather_code"] = true;
        filter[4]["hourly"]["surface_pressure"] = true;
        filter[5]["hourly"]["wind_speed_10m"] = true;
        filter[6]["hourly"]["wind_direction_10m"] = true;
        uint8_t timePoint[48];
        for(uint8_t f=0; f<7; f++) {
            JsonDocument forecast;
            DeserializationError errorForecast = deserializeJson(forecast, httpData, DeserializationOption::Filter(filter[f]));
            if(errorForecast) {
                Serial.print("Open-meteo.com: hourly forecast " + String(f) + " deserialization error: ");
                return;
            }
            uint8_t n = 0;
            for(uint8_t i=0; i<144; i++) {
                if(f == 0) {
                    time_t time = forecast["hourly"]["time"][i] | 0;
                    time_t utc = forecast["utc_offset_seconds"] | 0;
                    uint8_t hr = hour(time + utc);
                    if(hr % 3 == 0 && (time + utc) >= now()) {
                        state.weather.hourly.date[n] = forecast["hourly"]["time"][i] | 0;
                        state.weather.hourly.date[n] += utc;
                        timePoint[n] = i;
                        if(n<39) n++;
                        else break;
                    }
                }
                else if(timePoint[n] == i) {
                    if(f == 1) state.weather.hourly.temp[n] = forecast["hourly"]["temperature_2m"][i] | 40400.0;
                    if(f == 4) state.weather.hourly.pres[n] = forecast["hourly"]["surface_pressure"][i] | 40400.0;
                    if(f == 5) state.weather.hourly.windSpeed[n] = forecast["hourly"]["wind_speed_10m"][i] | -1.0;
                    if(f == 6) state.weather.hourly.windDir[n] = forecast["hourly"]["wind_direction_10m"][i] | 0.0;
                    if(f == 2) state.weather.hourly.prec[n] = forecast["hourly"]["precipitation_probability"][i] | 0.0;
                    if(f == 3) state.weather.hourly.icon[n] = _openMeteoIcon(forecast["hourly"]["weather_code"][i] | 0);
                    if(n<39) n++;
                    else break;
                }  
            }
        }
        
        httpData = "";
        state.weather.hourly.time = now();
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
        state.weather.daily.tMax[i] = -40400.0;
        state.weather.daily.tMin[i] = 40400.0;
        state.weather.daily.wind[i] = -1.0;
        state.weather.daily.icon[i] = 0;
    }
    for(unsigned int i=0; i<DAYS*8; i++) {
        unsigned int wd = weekday(state.weather.time);
        if(i == 0) {
            state.weather.daily.tMax[0] = state.weather.temp;
            state.weather.daily.tMin[0] = state.weather.temp;
            state.weather.daily.icon[0] = state.weather.icon;
            state.weather.daily.wind[0] = state.weather.wind.speed;
        }
        for(unsigned int k=0; k<DAYS; k++) {
            if(weekday(state.weather.hourly.date[i]) == wd) {
                if(state.weather.daily.tMax[k] < state.weather.hourly.temp[i]) state.weather.daily.tMax[k] = state.weather.hourly.temp[i];
                if(state.weather.daily.tMin[k] > state.weather.hourly.temp[i]) state.weather.daily.tMin[k] = state.weather.hourly.temp[i];
                if(state.weather.daily.icon[k] < state.weather.hourly.icon[i]) state.weather.daily.icon[k] = state.weather.hourly.icon[i];
                if(state.weather.daily.wind[k] < state.weather.hourly.windSpeed[i]) state.weather.daily.wind[k] = state.weather.hourly.windSpeed[i];
            }
            if(++wd > 7) wd = 1;
        }
    }
    Serial.println("done");
}

/**
 * Getters 
 */
float Weather::get_currentTemp(bool corr) {
    return state.weather.temp + (corr ? config.weather_temp_corr() : 0.0);
}

float Weather::get_currentHum(bool corr) {
    return state.weather.hum + (corr ? config.weather_hum_corr() : 0.0);
}

float Weather::get_currentPres(bool corr) {
    return _state.weather.pres + (corr ? config.weather_pres_corr() : 0.0);
}

float Weather::get_currentWindSpeed() {
    return state.weather.wind.speed;
}

int Weather::get_currentWindDir() {
    return state.weather.wind.dir;
}

unsigned int Weather::get_currentIcon() {
    return _convertIcon(state.weather.icon);
}

bool Weather::get_isDay() {
    return state.weather.isDay;
}

String Weather::get_description() {
    return String(state.weather.descript);
}

time_t Weather::get_currentUpdated() {
    return state.weather.time;
}

float Weather::get_dailyDayTemp(unsigned int num) {
    if(num >= DAYS) return 40400.0;
    return state.weather.daily.tMax[num];
}

float Weather::get_dailyNightTemp(unsigned int num) {
    if(num >= DAYS) return 40400.0;
    return state.weather.daily.tMin[num];
}

float Weather::get_dailyWindSpeed(unsigned int num) {
    if(num >= DAYS) return -1.0;
    return state.weather.daily.wind[num];
}

unsigned int Weather::get_dailyIcon(unsigned int num) {
    if(num >= DAYS) return 0;
    return _convertIcon(state.weather.daily.icon[num]);
}

unsigned int Weather::get_hourlyDate(unsigned int num) {
    if(num >= DAYS * 8) return 0;
    return state.weather.hourly.date[num];
}

unsigned int Weather::get_hourlyIcon(unsigned int num) {
    if(num >= DAYS * 8) return 0;
    return state.weather.hourly.icon[num];
}

float Weather::get_hourlyTemp(unsigned int num) {
    if(num >= DAYS * 8) return 40400.0;
    return state.weather.hourly.temp[num];
}

float Weather::get_hourlyPres(unsigned int num) {
    if(num >= DAYS * 8) return 40400.0;
    return state.weather.hourly.pres[num];
}

float Weather::get_hourlyWindSpeed(unsigned int num) {
    if(num >= DAYS * 8) return -1.0;
    return state.weather.hourly.windSpeed[num];
}

int Weather::get_hourlyWindDir(unsigned int num) {
    if(num >= DAYS * 8) return -1;
    return state.weather.hourly.windDir[num];
}

float Weather::get_hourlyPrec(unsigned int num) {
    if(num >= DAYS * 8) return -1.0;
    return state.weather.hourly.prec[num];
}