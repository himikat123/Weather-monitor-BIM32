/* Openweathermap streaming parser
 * based on https://github.com/sfrwmaker/openweathermap.org-ESP8266
 */

#include <JsonListener.h>        //
#include <JsonStreamingParser.h> // v1.0.5 https://github.com/squix78/json-streaming-parser

/* 
 * Universal weather request parser 
 */
#define OWM_max_layers 4
class OWMrequest: public JsonListener {
  public:
    void init(void);
    OWMrequest() : JsonListener() { }
    virtual void key(String key) { currentKey = String(key); }
    virtual void endObject();
    virtual void startObject();
    virtual void whitespace(char c) { }
    virtual void startDocument() { }
    virtual void endArray() { }
    virtual void endDocument() { }
    virtual void startArray() { }
  protected:
    void doUpdate(String url);
    String currentKey;
    String currentParent;
    String p_key[OWM_max_layers];
};

/* 
 * Five day forecast 
 */
typedef struct sOWM_fiveForecast {
  String dt;
  String t_min;
  String t_max;
  String pressure;
  String icon;
  String w_speed;
  String w_deg;
  String cond;
  String cond_value;
} OWM_fiveForecast;

class OWMfiveForecast : public OWMrequest {
  public:
    OWMfiveForecast() { }
    uint8_t updateForecast(OWM_fiveForecast *forecasts, String apiKey, String location);
    virtual void value(String value);

  private:
    uint8_t index;
    uint32_t timestamp;
    uint8_t max_forecasts;
    OWM_fiveForecast *forecasts;
};

void OWMrequest::doUpdate(String url) {
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
    while(client -> connected()) {
      while(true) {
        if(client -> available() > 0) {
          c = client -> read();
          if(c == '{' || c == '[') isBody = true;
          if(isBody) parser.parse(c);
        }
        else {
          uint32_t now_ms = millis();
          while(millis() - now_ms < 200) {
            yield();
            if(client -> available() > 0) break;
          }
          if(client -> available() <= 0) break;
        }
        if(millis() - break_millis > 120000) break;
      }
      if(millis() - break_millis > 120000) break;
    }
  }
}

void OWMrequest::init(void) {
  for(byte i=0; i<OWM_max_layers; ++i) p_key[i] = "";
  currentParent = currentKey = "";
}

void OWMrequest::startObject() {
  for(byte i=OWM_max_layers-1; i>0; --i) p_key[i] = p_key[i-1];
  p_key[0] = currentParent;
  currentParent = currentKey;
}

void OWMrequest::endObject() {
  currentParent = p_key[0];
  for(byte i=0; i<OWM_max_layers-1; ++i) p_key[i] = p_key[i+1];
  currentKey = "";
}

/* 
 * Five day forecast 
 */
byte OWMfiveForecast::updateForecast(OWM_fiveForecast *forecasts, String apiKey, String location) {
  this -> forecasts = forecasts;
  OWMrequest::init();
  index = 0;
  timestamp = 0;

  String url = "http://api.openweathermap.org/data/2.5/forecast?" + location;
  url += "&units=metric";
  url += "&appid=" + apiKey;
  doUpdate(url);

  this -> forecasts = nullptr;
  return index;
}

void OWMfiveForecast::value(String value) {
  if(currentKey == "dt") {
    if(timestamp == 0) {
      index = 0;
      forecasts[index].dt = value;
      timestamp = value.toInt();
    }
    else {
      uint32_t t = value.toInt();
      if(t > timestamp) {
        if(index < max_forecasts - 1) {
          ++index;
          timestamp = t;
          forecasts[index].dt = value;
        }
      }
    }
  }
  else if(currentKey == "temp_min") forecasts[index].t_min = value;
  else if(currentKey == "temp_max") forecasts[index].t_max = value;
  else if(currentKey == "pressure") forecasts[index].pressure = value;
  else if(currentParent == "weather" && currentKey == "icon") forecasts[index].icon = value;
  else if(currentParent == "wind" && currentKey == "speed") forecasts[index].w_speed = value;
  else if(currentParent == "wind" && currentKey == "deg") forecasts[index].w_deg = value;
  else if(currentParent == "rain" && currentKey == "3h") {
    forecasts[index].cond = currentParent;
    forecasts[index].cond_value = value;
  }
  else if(currentParent == "snow" && currentKey == "3h") {
    forecasts[index].cond = currentParent;
    forecasts[index].cond_value = value;
  }
}
