#include <HTTPClient.h>
#include <WiFiClient.h>
#include <TimeLib.h>

#include "./weather.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../languages/languages.hpp"
#include "../globals.hpp"

/**
 * Gismeteo weather update
 */
void Weather::_updateGismeteo() {
    String lng = config.lang();
    if(lng == "de" || lng == "bg" || lng == "es") lng = "en";

    String url = "http://services.gismeteo.net/inform-service/inf_chrome/forecast/?lang=";
    url += lng;
    url += "&city=";
    url += String(config.weather.cityid());

    //Serial.print("URL: "); Serial.println(url);
    WiFiClient client;
    HTTPClient http;
    http.end(); 

    if(!http.begin(client, url)) return;
    http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64)");
    http.setTimeout(5000); 

    int httpCode = http.GET();

    if(httpCode == HTTP_CODE_OK) {
        WiFiClient* stream = http.getStreamPtr();
        _parseGismeteoStream(stream);
        state.weather.time = now();
        state.weather.daily.time = now();
        state.weather.hourly.time = now();
        state.weather.updated = true;
        Serial.print("Current weather updated successfully at: ");
        Serial.printf("%02d:%02d:%02d\r\n", hour(), minute(), second());
    }
    else Serial.printf("Weather update error: %d\n", httpCode);
    http.end();
}

void Weather::_parseGismeteoStream(WiFiClient* stream) {
    String currentBlockType = ""; 
    String dateOrTime = "";       
    bool insideDayBlock = false;
    unsigned int dayNr = 0;
    unsigned int forecastCount = 0;
    time_t current = now(); 
    time_t midnight = previousMidnight(current); 

    stream->setTimeout(2000);

    while(stream->connected() || stream->available()) {
        int retry = 0;
        while(!stream->available() && stream->connected() && retry < 50) {
            vTaskDelay(10);
            retry++;
        }

        if(!stream->available()) break;

        String block = stream->readStringUntil('>');
        block.trim();

        if(block.length() == 0) continue;
        if(block.indexOf("</weather") != -1 || block.indexOf("</location") != -1) break;

        if(block.indexOf("<fact") != -1) {
            currentBlockType = "fact";
            time_t sunrise = _getXmlAttr(block, "sunrise").toInt();
            time_t sunset  = _getXmlAttr(block, "sunset").toInt();
            state.weather.isDay = (sunrise <= now() && now() <= sunset) ? true : false;
        }
        else if(block.indexOf("<forecast") != -1) {
            currentBlockType = "forecast";
            dateOrTime = _getXmlAttr(block, "valid");
        }
        else if(block.indexOf("</fact>") != -1 || block.indexOf("</forecast>") != -1) {
            currentBlockType = "";
        }
        else if(block.indexOf("<values") != -1 && currentBlockType != "") {
            String temp = _getXmlAttr(block, "t");
            String pressure = _getXmlAttr(block, "p");
            String humidity = _getXmlAttr(block, "hum");
            String windSpeed = _getXmlAttr(block, "ws");
            String windDirId = _getXmlAttr(block, "wd");
            String desc = _getXmlAttr(block, "descr");
            String icon = _getXmlAttr(block, "icon");
            String precip = _getXmlAttr(block, "pr");

            if(currentBlockType == "fact") {
                state.weather.temp       = temp.toFloat();
                state.weather.hum        = humidity.toFloat();
                state.weather.pres       = pressure.toFloat() / 0.750062; 
                state.weather.wind.speed = windSpeed.toFloat();
                state.weather.wind.dir   = _parseGismeteoWindDir(windDirId.toInt());
                
                int iconId = _convertGismeteoIcon(icon);
                state.weather.icon = iconId;

                if(config.lang() == "de" || config.lang() == "bg" || config.lang() == "es") {
                    int descIdx = _getDescIdxFromIcon(iconId);
                    strncpy(state.weather.descript, lang.weatherDescription(descIdx), sizeof(state.weather.descript) - 1);
                    state.weather.descript[sizeof(state.weather.descript) - 1] = '\0';
                }
                else {
                    desc.toCharArray(state.weather.descript, sizeof(state.weather.descript));
                }

                state.weather.time = now();
            }
            else if(currentBlockType == "forecast") {
                int hourIconId = _convertGismeteoIcon(icon);
                unsigned long offsetSeconds = forecastCount * 10800UL;
                time_t targetTimestamp = midnight + offsetSeconds;
                
                state.weather.hourly.date[forecastCount] = targetTimestamp;
                state.weather.hourly.icon[forecastCount] = hourIconId;
                state.weather.hourly.temp[forecastCount] = temp.toFloat();
                state.weather.hourly.pres[forecastCount] = pressure.toFloat() / 0.750062;
                state.weather.hourly.windSpeed[forecastCount] = windSpeed.toFloat();
                state.weather.hourly.windDir[forecastCount] = _parseGismeteoWindDir(windDirId.toInt());
                state.weather.hourly.prec[forecastCount] = precip.toFloat();
                forecastCount++;
            }
        }
        else if(block.indexOf("<day") != -1) {
            String dayDate = _getXmlAttr(block, "date");
            if(dayDate != "") {
                if(!insideDayBlock) insideDayBlock = true;
                String dayIconRaw = _getXmlAttr(block, "icon");
                int dayIconId = _convertGismeteoIcon(dayIconRaw);

                if(dayNr < DAYS) {
                    state.weather.daily.tMax[dayNr] = _getXmlAttr(block, "tmax").toFloat();
                    state.weather.daily.tMin[dayNr] = _getXmlAttr(block, "tmin").toFloat();
                    state.weather.daily.wind[dayNr] = _getXmlAttr(block, "wsmax").toFloat();
                    state.weather.daily.icon[dayNr++] = dayIconId;
                }
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

int Weather::_convertGismeteoIcon(const String& gisIcon) {
    if(gisIcon.length() == 0) return 1;
    if(gisIcon.indexOf("ts") != -1 || gisIcon.indexOf("t") != -1) return 11;
    else if(gisIcon.indexOf("s") != -1) return 13;
    else if(gisIcon.indexOf("r") != -1) {
        if(gisIcon.indexOf("r3") != -1 || gisIcon.indexOf("r2") != -1) return 9;
        return 10;
    }
    else if(gisIcon.indexOf("f") != -1) return 50;
    else if(gisIcon.indexOf("c") != -1) {
        if(gisIcon.indexOf("c3") != -1 || gisIcon.indexOf("c4") != -1) return 3;
        return 2;
    }
    return 1;
}

int Weather::_getDescIdxFromIcon(int iconId) {
    switch (iconId) {
        case 1:  return 0;  // Clear sky
        case 2:  return 2;  // Partly cloudy
        case 3:  return 3;  // Overcast
        case 9:  return 12; // Rain showers
        case 10: return 8;  // Rain
        case 11: return 14; // Thunderstorm
        case 13: return 10; // Snowfall
        case 50: return 4;  // Fog
        default: return 0;
    }
}