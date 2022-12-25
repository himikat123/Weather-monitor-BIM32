/* Openweathermap streaming parser
 * based on https://github.com/sfrwmaker/openweathermap.org-ESP8266
 */


#include <JsonListener.h>
#include <JsonStreamingParser.h>

//------------------------------------------ Universal weather request parser for opewWeatherMap site ---------
#define OWM_max_layers 4
class OWMrequest: public JsonListener {
  public:
    void init(void);
    OWMrequest() : JsonListener()                   { }
    virtual void key(String key)                    { currentKey = String(key); }
    virtual void endObject();
    virtual void startObject();
    virtual void whitespace(char c)                 { }
    virtual void startDocument()                    { }
    virtual void endArray()                         { }
    virtual void endDocument()                      { }
    virtual void startArray()                       { }
  protected:
    void   doUpdate(String url, uint8_t maxForecasts = 0);
    String currentKey;
    String currentParent;
    String p_key[OWM_max_layers];
};

//------------------------------------------ Current weather conditions from openweatrhermap.org --------------
typedef struct sOWM_conditions {
  String longtitude;
  String latitude;
  String id;
  String main;
  String description;
  String icon;
  String temp;
  String pressure;
  String humidity;
  String t_min;
  String t_max;
  String visibility;
  String w_speed;
  String w_deg;
  String cond;                                      // conditions: cloud, rain, snow
  String cond_value;
  String dt;
  String sunrise;
  String sunset;
} OWM_conditions;

class OWMconditions : public OWMrequest {
  public:
    OWMconditions()                                { currentParent = ""; }
    void    init(void)                             { currentParent = ""; }
    void    updateConditions(OWM_conditions *conditions, String apiKey,
    String  country, String city, String units = "", String language = "");
    virtual void value(String value);

  private:
    OWM_conditions *conditions;
};


//------------------------------------------ Five day forecast from openweatrhermap.org -----------------------
typedef struct sOWM_fiveForecast {
  String dt;
  String temp;
  String t_min;
  String t_max;
  String pressure;
  String sea_pressure;
  String humidity;
  String id;
  String main;
  String description;
  String icon;
  String w_speed;
  String w_deg;
  String cond;                                      // conditions: cloud, rain, snow
  String cond_value;
} OWM_fiveForecast;

class OWMfiveForecast : public OWMrequest {
  public:
    OWMfiveForecast()                               { }
    uint8_t updateForecast(OWM_fiveForecast *forecasts, uint8_t maxForecasts, String apiKey,
              String location, String units = "", String language = "");
    virtual void value(String value);

  private:
    uint8_t     index;
    uint32_t   timestamp;
    uint8_t     max_forecasts;
    OWM_fiveForecast *forecasts;
};

void OWMrequest::doUpdate(String url, byte maxForecasts) {
  JsonStreamingParser parser;
  parser.setListener(this);

  HTTPClient http;

  Serial.println(url);
  http.begin(url);
  bool isBody = false;
  char c;
  int httpCode = http.GET();
  Serial.println("code:" + String(httpCode));
  if(httpCode > 0) {
    WiFiClient * client = http.getStreamPtr();
    uint32_t break_millis = millis(); 
    while(client->connected()) {
      while(true) {
        if (client->available() > 0) {
          c = client->read();
          if (c == '{' || c == '[') {
            isBody = true;
          }
          if (isBody) {
            parser.parse(c);
          }
        } else {
          uint32_t now_ms = millis();
          while (millis() - now_ms < 200) {
            yield();
            if (client->available() > 0)
              break;
          }
          if (client->available() <= 0)
            break;
        }
        if(millis() - break_millis > 120000) break;
      }
      if(millis() - break_millis > 120000) break;
    }
  }
}

void OWMrequest::init(void) {
  for (byte i = 0; i < OWM_max_layers; ++i)
    p_key[i] = "";
  currentParent = currentKey = "";
}

void OWMrequest::startObject() {
  for (byte i = OWM_max_layers-1; i > 0; --i) {
    p_key[i] = p_key[i-1];
  }
  p_key[0] = currentParent;
  currentParent = currentKey;
}

void OWMrequest::endObject() {
  currentParent = p_key[0];
  for (byte i = 0; i < OWM_max_layers-1; ++i) {
    p_key[i] = p_key[i+1];
  }
  currentKey = "";
}

void OWMconditions::updateConditions(OWM_conditions *conditions, String apiKey, String country, String city, String units, String language) {
  this->conditions = conditions;
  OWMrequest::init();

  String url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + country;
  if (language != "") url += "&lang="  + language;
  if (units != "")    url += "&units=" + units;
  url +=  + "&appid=" + apiKey;
  doUpdate(url);
  this->conditions = nullptr;
}

void OWMconditions::value(String value) {
  if (currentParent == "coord") {
    if (currentKey == "lon") {
      conditions->longtitude = value;
    } else if (currentKey == "lat") {
      conditions->latitude = value;
    }
  } else if (currentParent == "weather") {
    if (currentKey == "id") {
      conditions->id = value;
    } else if (currentKey == "main") {
      conditions->main = value;
    } else if (currentKey == "description") {
      conditions->description = value;
    } else if (currentKey == "icon") {
      conditions->icon = value;
    }
  } else if (currentParent == "main") {
    if (currentKey == "temp") {
      conditions->temp = value;
    } else if (currentKey == "pressure") {
      conditions->pressure = value;
    } else if (currentKey == "humidity") {
      conditions->humidity = value;
    } else if (currentKey == "temp_min") {
      conditions->t_min = value;
    } else if (currentKey == "temp_max") {
      conditions->t_max = value;
    }
  } else if (currentParent == "wind") {
    if (currentKey == "speed") {
      conditions->w_speed = value;
    } else if (currentKey == "deg") {
      conditions->w_deg = value;
    }
  } else if (currentParent == "clouds") {
    if (currentKey == "all") {
      conditions->cond = currentParent;
      conditions->cond_value = value;
    }
  } else if (currentParent == "rain") {
    if (currentKey == "3h") {
      conditions->cond = currentParent;
      conditions->cond_value = value;
    }  
  } else if (currentParent == "snow") {
    if (currentKey == "3h") {
      conditions->cond = currentParent;
      conditions->cond_value = value;
    } 
  } else if (currentParent == "sys") {
    if (currentKey == "sunrise") {
      conditions->sunrise = value;
    } else if (currentKey == "sunset") {
      conditions->sunset = value;
    }
  } else {
    if (currentKey == "visibility") {
      conditions->visibility = value;
    } else if (currentKey == "dt") {
      conditions->dt = value;
    } 
  }
}

//------------------------------------------ Five day forecast from openweatrhermap.org -----------------------
byte OWMfiveForecast::updateForecast(OWM_fiveForecast *forecasts, byte maxForecasts, String apiKey, String location, String units, String language) {
  this->forecasts = forecasts;
  this->max_forecasts = maxForecasts;
  OWMrequest::init();
  index = 0;
  timestamp = 0;

  String url = "http://api.openweathermap.org/data/2.5/forecast?"+ location;
  if (language != "") url += "&lang="  + language;
  if (units != "")    url += "&units=" + units;
  url +=  + "&appid=" + apiKey;
  doUpdate(url, maxForecasts);

  this->forecasts = nullptr;
  return index;
}

void OWMfiveForecast::value(String value) {
  if (currentKey == "dt") {
    if (timestamp == 0) {
      index = 0;
      forecasts[index].dt = value;
      timestamp = value.toInt();
    } else {
      uint32_t t = value.toInt();
      if (t > timestamp) {                          // new forecast time
        if (index < max_forecasts - 1) {
          ++index;
          timestamp = t;
          forecasts[index].dt = value;
        }
      }
    }
  } else
  if (currentKey == "temp") {
    forecasts[index].temp = value;
  } else
  if (currentKey == "temp_min") {
    forecasts[index].t_min = value;
  } else
  if (currentKey == "temp_max") {
    forecasts[index].t_max = value;
  } else
  if (currentKey == "pressure") {
    forecasts[index].pressure = value;
  } else
  if (currentKey == "sea_level") {
    forecasts[index].sea_pressure = value;
  } else
  if (currentKey == "humidity") {
    forecasts[index].humidity = value;
  } else
  if (currentParent == "weather" && currentKey == "description") {
    forecasts[index].description = value;
  } else
  if (currentParent == "weather" && currentKey == "icon") {
    forecasts[index].icon = value;
  } else
  if (currentParent == "weather" && currentKey == "id") {
    forecasts[index].id = value;
  } else
  if (currentParent == "wind" && currentKey == "speed") {
    forecasts[index].w_speed = value;
  } else
  if (currentParent == "wind" && currentKey == "deg") {
    forecasts[index].w_deg = value;
  } else
  if (currentParent == "clouds" && currentKey == "all") {
    forecasts[index].cond = currentParent;
    forecasts[index].cond_value = value;
  } else
  if (currentParent == "rain" && currentKey == "3h") {
    forecasts[index].cond = currentParent;
    forecasts[index].cond_value = value;
  }  else
  if (currentParent == "snow" && currentKey == "3h") {
    forecasts[index].cond = currentParent;
    forecasts[index].cond_value = value;
  }
}
