#include <HTTPClient.h>
#include <WiFiClient.h>
#include <TimeLib.h>

#include "./weather.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"

/**
 * Gismeteo weather update
 */
void Weather::_updateGismeteo() {
    String url = "http://services.gismeteo.net/inform-service/inf_chrome/forecast/?lang=ru&city=";
    url += String(config.weather.cityid());
    Serial.printf("URL: %s\n", url);
    static WiFiClient client;
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
    http.setTimeout(5000); 

    int httpCode = http.GET();

    if(httpCode == HTTP_CODE_OK) {
        WiFiClient* stream = http.getStreamPtr();
        _parseGismeteoStream(stream);
    } 
    else Serial.printf("Ошибка HTTP запроса: %d\n", httpCode);
    http.end();
}


void Weather::_parseGismeteoStream(WiFiClient* stream) {
    int forecastCount = 0;
    String currentBlockType = ""; 
    //String dateOrTime = "";       
    bool insideDayBlock = false;
    //String timestamp = "", dayDate = "", dayTmin = "", dayTmax = "", dayWindMax = "", dayHumMax = "", dayDesc = "";

    stream->setTimeout(2000);

    while(stream->connected() || stream->available()) {
        if(!stream->available()) {
            vTaskDelay(10);
            continue;
        }

        String block = stream->readStringUntil('>');
        block.trim();

        if(block.length() == 0) continue;

        if(block.indexOf("<fact") != -1) currentBlockType = "fact";
        else if(block.indexOf("<forecast") != -1) currentBlockType = "forecast";

        else if(block.indexOf("<values") != -1 && currentBlockType != "") {
            String temp = _getXmlAttr(block, "t");
            String pressure = _getXmlAttr(block, "p");
            String humidity = _getXmlAttr(block, "hum");
            String windSpeed = _getXmlAttr(block, "ws");
            String windDirId = _getXmlAttr(block, "wd");
            String desc = _getXmlAttr(block, "descr");
            String icon = _getXmlAttr(block, "icon");

            if(currentBlockType == "fact") {
                desc.toCharArray(state.weather.descript, sizeof(state.weather.descript));
                state.weather.temp       = temp.toFloat();
                state.weather.hum        = humidity.toFloat();
                state.weather.pres       = pressure.toFloat() / 0.75;
                state.weather.wind.speed = windSpeed.toFloat();
                state.weather.wind.dir   = _parseGismeteoWindDir(windDirId.toInt());
                state.weather.icon       = 0;
                time_t sunrise           = _getXmlAttr(block, "sunrise").toInt();
                time_t sunset            = _getXmlAttr(block, "sunset").toInt();
                state.weather.isDay      = (sunrise <= now() && now() <= sunset) ? true : false;
                state.weather.time       = now();

                Serial.printf("Температура: %f °C\n", state.weather.temp);
                Serial.printf("Давление: %f hPa\n", state.weather.pres);
                Serial.printf("Влажность: %f %%\n", state.weather.hum);
                Serial.printf("Ветер: %f м/с, направление: %d\n", state.weather.wind.speed, state.weather.wind.dir);
                Serial.printf("Описание: %s\n", state.weather.descript);
                Serial.printf("Иконка: %s\n", icon);
            }

            else if (currentBlockType == "forecast") {
                forecastCount++;
                //Serial.printf("%02d. Время: %s | Т: %s°C | Давление: %s | Ветер: %s м/с (%s)\n", 
                //    forecastCount, dateOrTime.c_str(), temp.c_str(), pressure.c_str(), windSpeed.c_str(), windDirStr.c_str());
            }
            currentBlockType = "";
        }

        else if (block.indexOf("<day") != -1) {
            String dayDate = _getXmlAttr(block, "date");
            if(dayDate != "") {
                if(!insideDayBlock) {
                    Serial.println("-----------------------------------------");
                    Serial.println("[ ДОЛГОСРОЧНЫЙ ПРОГНОЗ ПО ДНЯМ ]");
                    insideDayBlock = true;
                }
                Serial.printf("temp min: %f\n", _getXmlAttr(block, "tmin"));
                Serial.printf("temp max: %f\n", _getXmlAttr(block, "tmax"));
                Serial.printf("temp wind max: %f\n", _getXmlAttr(block, "wsmax"));
            }
        }
    }
}

String Weather::_getXmlAttr(const String& block, const String& attrName) {
    String searchStr = attrName + "=\"";
    int startIdx = block.indexOf(searchStr);
    if(startIdx == -1) return "";

    startIdx += searchStr.length();
    int endIdx = block.indexOf("\"", startIdx);
    if(endIdx == -1) return "";

    return block.substring(startIdx, endIdx);
}

int Weather::_parseGismeteoWindDir(int id) {
    switch (id) {
        case 2: return 45;
        case 3: return 90;
        case 4: return 135;
        case 5: return 180;
        case 6: return 225;
        case 7: return 270;
        case 8: return 315;
        default: return 0;
    }
}