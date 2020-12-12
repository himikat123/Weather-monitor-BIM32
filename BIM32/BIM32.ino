/* 
 *  Weather Monitor BIM32 v1.1.4
 *  © himikat123@gmail.com, Nürnberg, Deutschland, 2020
 */
 
        // 1.9 MB APP with OTA / 190 kB SPIFFS
        
#include <Arduino.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include <FS.h>
#include <SPIFFS.h>
#include <Update.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <OpenWeatherMap.h>
#include "EasyNextionLibrary.h"
#include "ThingSpeak.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "bim32.h"
#include <NeoPixelBus.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
#define FS_NO_GLOBALS

WiFiClient client;
EasyNex myNex(Serial1);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);
RgbColor red(colorSaturation, 0, 0);
RgbColor yellow(colorSaturation, colorSaturation, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor cyan(0, colorSaturation, colorSaturation);
RgbColor blue(0, 0, colorSaturation);
RgbColor purple(colorSaturation, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);


void setup(){
  pinMode(SET_HC12, OUTPUT);
  uint8_t cardType;
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(9600);
  myNex.writeStr("rest");
    
  WiFi.mode(WIFI_STA);
  SPIFFS.begin();

  read_config();

  xTaskCreatePinnedToCore(TaskDisplay, "TaskDisplay", 32768, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskHC12rcv, "TaskHC12rcv", 16384, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskSensors, "TaskSensors", 16384, NULL, 2, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskWeather, "TaskWeather", 32768, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskWS2812B, "TaskWS2812B", 16384, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  if(config.mqttsend or config.mqttrcv){
    xTaskCreatePinnedToCore(TaskHC12MQTT, "TaskHC12MQTT", 32768, NULL, 1, NULL, ARDUINO_RUNNING_CORE);
  }
}

void loop(){}

void TaskDisplay(void *pvParameters){
  (void) pvParameters;
  uint32_t but = 0;
  uint32_t old_bright = millis();
  vTaskDelay(1000);
  myNex.writeStr("page page1");
  datas.page = 1;
  while(1){
    if(digitalRead(0) == 0){
      if(millis() - but > 1000){
        if(datas.is_settings){
          datas.is_settings = false;
          myNex.writeStr("page page1");
        }
        else{
          datas.is_settings = true;
          myNex.writeStr("page page7");
        }
      }
      but = millis();
    }
    disp_receive();
    if(datas.page == 1){
      int8_t ant_diff = (datas.old_ant > datas.ant) ? (datas.old_ant - datas.ant) : (datas.ant - datas.old_ant);
      if(datas.old_htemp != datas.temp[1] or
         datas.old_hhum != datas.hum[1] or
         ant_diff > 3 or
         datas.old_icon != datas.icon[0] or
         datas.old_otemp != datas.temp[0] or
         datas.old_ohum != datas.hum[0] or
         datas.old_opres != datas.pres or
         datas.old_w_speed != datas.w_speed[0] or
         datas.old_temp_d != datas.temp[2] or
         datas.old_temp_n != datas.temp[3]
       ) page1_send();
    }
    if(datas.old_ant != datas.ant and datas.page == 3) page3_send();

    if(datas.clock_synchronized){
      datas.clock_synchronized = false;
      if(now() > 1594249719){
        myNex.writeNum("rtc0", year());
        myNex.writeNum("rtc1", month());
        myNex.writeNum("rtc2", day());
        myNex.writeNum("rtc3", hour());
        myNex.writeNum("rtc4", minute());
        myNex.writeNum("rtc5", second());
      }
    }

    if(datas.page != 9){
      if(config.brt == 0){
        if(datas.isDay) datas.bright = config.brday;
        else datas.bright = config.brnight;
      }
      if(config.brt == 1){
        datas.bright = round(datas.light) * 3;
      }
      if(config.brt == 2){
        uint16_t sunrise = config.hd * 60 + config.md;
        uint16_t sunset = config.hn * 60 + config.mn;
        uint16_t cur_time = hour() * 60 + minute();
        if(cur_time > sunrise and cur_time < sunset){
          datas.isDay = true;
          datas.bright = config.brday;
        }
        else{
          datas.isDay = false;
          datas.bright = config.brnight;
        }
      }
      if(config.brt == 3){
        datas.bright = config.brday;
      }
      if(datas.bright < 1) datas.bright = 1;
      if(datas.bright > 100) datas.bright = 100;
      if(millis() - old_bright > 1000){
        myNex.writeNum("dim", datas.bright);
        old_bright = millis();
      }
    }
        
    vTaskDelay(100);
  }
}


void TaskSensors(void *pvParameters){
  (void) pvParameters;
  if(config.ssid != "") wifi_connect();
  setSyncProvider(getTime);
  setSyncInterval(300);
  ThingSpeak.begin(client);
  sensors_init();
  sensors_read();
  data_ordering();
  uint32_t thngspk_update = millis() + 60000;
  uint32_t sensors_update = millis();
  uint32_t cloud_send = millis() + 300000;
  while(1){
    if((millis() - sensors_update) > 1000){
      sensors_update = millis();
      sensors_read();
    }

    if(config.ssid != ""){
      if(WiFi.status() != WL_CONNECTED){
        datas.net_connected = false;
        wifi_connect();
      }
      else{
        datas.net_connected = true;
        if(String(WiFi.SSID()) != String(config.ssid)){
          WiFi.disconnect();
          datas.net_connected = false;
          wifi_connect();
        }
        scan_nets();
      }
    }

    if(config.thngrcv){
      if((millis() - thngspk_update + 30000) > 60000){
        thingspk_recv();
        thngspk_update = millis();
      }
    }

    data_ordering();

    if((millis() - cloud_send) > (config.tupd * 60000)){
      if(config.thngsend) thingspk_send();
      if(config.nrdmsend) narodmon_send();
      vTaskDelay(200);
      if(config.mqttsend) datas.mqtt_sending = true;
      cloud_send = millis();
    }

    vTaskDelay(200);
  }
}

void TaskWeather(void *pvParameters){
  (void) pvParameters;
  while(1){
    if(datas.net_connected){
      getWeatherNow();
      getWeatherDaily();

      vTaskDelay(1200000);
    }
    else vTaskDelay(10);
  }
}

void data_ordering(void){
  switch(config.temp_out){
    case 0: datas.temp[0] = datas.temp_web; break;
    case 1: datas.temp[0] = ((now() - datas.time_wsens) < (config.wexpire * 60)) ? datas.temp_wsens : datas.temp_web; break;
    case 2: datas.temp[0] = sensors.bme280_temp; break;
    case 3: datas.temp[0] = sensors.bmp180_temp; break;
    case 4: datas.temp[0] = sensors.sht21_temp; break;
    case 5: datas.temp[0] = sensors.dht22_temp; break;
    case 8: datas.temp[0] = datas.mqtt_temp_out; break;
    case 9: datas.temp[0] = datas.thng_temp_out; break;
    default: datas.temp[0] = datas.temp_web; break;
  }
  datas.temp[0] = round(datas.temp[0] * 10.0) / 10.0;
  switch(config.hum_out){
    case 0: datas.hum[0] = datas.hum_web; break;
    case 1: datas.hum[0] = ((now() - datas.time_wsens) < (config.wexpire * 60)) ? round(datas.hum_wsens) : datas.hum_web; break;
    case 2: datas.hum[0] = round(sensors.bme280_hum); break;
    case 4: datas.hum[0] = round(sensors.sht21_hum); break;
    case 5: datas.hum[0] = round(sensors.dht22_hum); break;
    case 8: datas.hum[0] = round(datas.mqtt_hum_out); break;
    case 9: datas.hum[0] = round(datas.thng_hum_out); break;
    default: datas.hum[0] = datas.hum_web; break;
  }
  switch(config.pres_out){
    case 0: datas.pres = datas.pres_web; break;
    case 1: datas.pres = ((now() - datas.time_wsens) < (config.wexpire * 60)) ? round(datas.pres_wsens) : datas.pres_web; break;
    case 2: datas.pres = round(sensors.bme280_pres); break;
    case 3: datas.pres = round(sensors.bmp180_pres); break;
    case 8: datas.pres = round(datas.mqtt_pres_out); break;
    case 9: datas.pres = round(datas.thng_pres_out); break;
    default: datas.pres = datas.pres_web * 0.75; break;
  }
  switch(config.temp_in){
    case 0: datas.temp[1] = datas.temp_web; break;
    case 1: datas.temp[1] = ((now() - datas.time_wsens) < (config.wexpire * 60)) ? datas.temp_wsens : 404.0; break;
    case 2: datas.temp[1] = sensors.bme280_temp; break;
    case 3: datas.temp[1] = sensors.bmp180_temp; break;
    case 4: datas.temp[1] = sensors.sht21_temp; break;
    case 5: datas.temp[1] = sensors.dht22_temp; break;
    case 8: datas.temp[1] = datas.mqtt_temp_in; break;
    case 9: datas.temp[1] = datas.thng_temp_in; break;
    default: datas.temp[1] = 404.0; break;
  }
  datas.temp[1] = round(datas.temp[1] * 10.0) / 10.0;
  switch(config.hum_in){
    case 0: datas.hum[1] = datas.hum_web; break;
    case 1: datas.hum[1] = ((now() - datas.time_wsens) < (config.wexpire * 60)) ? round(datas.hum_wsens) : 202.0; break;
    case 2: datas.hum[1] = round(sensors.bme280_hum); break;
    case 4: datas.hum[1] = round(sensors.sht21_hum); break;
    case 5: datas.hum[1] = round(sensors.dht22_hum); break;
    case 8: datas.hum[1] = round(datas.mqtt_hum_in); break;
    case 9: datas.hum[1] = round(datas.thng_hum_in); break;
    default: datas.hum[1] = 202.0; break;
  }
  switch(config.light_in){
    case 1: datas.light = ((now() - datas.time_wsens) < (config.wexpire * 60)) ? datas.light_wsens : 1000; break;
    case 6: datas.light = sensors.max44009_light; break;
    case 7: datas.light = sensors.bh1750_light; break;
    case 8: datas.light = datas.mqtt_light_in; break;
    case 9: datas.light = datas.thng_light_in; break;
    default: datas.light = 1000.0; break;
  }
}

void wifi_connect(void){
  if(config.ssid != ""){
    int i = 0;
    WiFi.begin(config.ssid, config.pass);
    while(WiFi.status() != WL_CONNECTED){
      i++;
      vTaskDelay(500);
      if(i > 100) ESP.restart();
    }
    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);
    if(config.type){
      IPAddress ip;
      IPAddress subnet;
      IPAddress gateway;
      IPAddress dns1;
      IPAddress dns2;
      if(ip.fromString(config.ip) and
         gateway.fromString(config.gw) and
         subnet.fromString(config.mask) and
         dns1.fromString(config.dns1) and
         dns2.fromString(config.dns2)
      ) WiFi.config(ip, gateway, subnet, dns1, dns2);
    }
  }
}

void disp_receive(void){
  while(Serial1.available()){
    String disp = Serial1.readString();
    Serial.print(disp);
    if(disp.lastIndexOf("{") != -1){
      StaticJsonDocument<1024> root;
      DeserializationError error = deserializeJson(root, disp);
      if(!error){
        if(disp.lastIndexOf("page") != -1){
          datas.page = root["page"].as<int>();
          if(datas.page == 1){
            setTime(
              root["h"].as<int>()|hour(),
              root["i"].as<int>()|minute(),
              root["s"].as<int>()|second(),
              root["d"].as<int>()|day(),
              root["m"].as<int>()|month(),
              root["y"].as<int>()|year()
            );
            page1_send();
          }
          if(datas.page == 3) page3_send();
          if(datas.page == 8) page8_send();
          if(datas.page == 9) page9_send();
          if(datas.page == 10) page10_send();
          if(datas.page == 11) page11_send();
          if(datas.page == 12) page12_send();
          if(datas.page == 13) page13_send();
          if(datas.page == 130) sens_data_send();
          if(datas.page == 14) page14_send();
          if(datas.page == 140){
            uint8_t k = root["vbat_corr"];
            config.bat_k = 300 - k;
            datas.bat_voltage = (float)datas.bat_adc / (float)config.bat_k;
            wsens_data_send();
          }
          if(datas.page == 15) page15_send();
          if(datas.page == 16) page16_send();
          if(datas.page == 17) page17_send();
          if(datas.page == 18) page18_send();
          if(datas.page == 19) page19_send();
          if(datas.page == 20) page20_send();
          if(datas.page == 21) page21_send();
          if(datas.page == 22) page22_send();
          if(datas.page == 23) page23_send();
          if(datas.page == 24) page24_send();
          if(datas.page == 240) page24a_send();
        }
        if(disp.lastIndexOf("save") != -1){
          strlcpy(config.ssid, root["ssid"] | config.ssid, sizeof(config.ssid));
          strlcpy(config.pass, root["pass"] | config.pass, sizeof(config.pass));
          strlcpy(config.mask, root["mask"] | config.mask, sizeof(config.mask));
          strlcpy(config.dns1, root["dns1"] | config.dns1, sizeof(config.dns1));
          strlcpy(config.dns2, root["dns2"] | config.dns2, sizeof(config.dns2));
          strlcpy(config.ip, root["ip"] | config.ip, sizeof(config.ip));
          strlcpy(config.gw, root["gw"] | config.gw, sizeof(config.gw));
          config.type = root["type"].as<bool>() | config.type;

          strlcpy(config.lang, root["lang"] | config.lang, sizeof(config.lang));
          config.brt = root["brt"] | config.brt;
          config.brday = int(root["brday"] | config.brday);
          config.brnight = int(root["brnight"] | config.brnight);
          config.hd = root["hd"] | config.hd;
          config.md = root["md"] | config.md;
          config.hn = root["hn"] | config.hn;
          config.mn = root["mn"] | config.mn;

          config.temp_out = root["temp_out"] | config.temp_out;
          config.hum_out = root["hum_out"] | config.hum_out;
          config.pres_out = root["pres_out"] | config.pres_out;
          config.temp_in = root["temp_in"] | config.temp_in;
          config.hum_in = root["hum_in"] | config.hum_in;
          config.light_in = root["light_in"] | config.light_in;
          config.bat_disp = root["bat_disp"] | config.bat_disp;

          config.utc = root["utc"] | config.utc;
          config.daylight = root["dlst"] | config.daylight;
          strlcpy(config.ntp, root["ntp"] | config.ntp, sizeof(config.ntp));

          strlcpy(config.city, root["city"] | config.city, sizeof(config.city));
          strlcpy(config.cityid, root["cityid"] | config.cityid, sizeof(config.cityid));
          strlcpy(config.lat, root["lat"] | config.lat, sizeof(config.lat));
          strlcpy(config.lon, root["lon"] | config.lon, sizeof(config.lon));
          strlcpy(config.appid, root["appid"] | config.appid, sizeof(config.appid));
          config.citysearch = root["citysearch"] | config.citysearch;

          sensors.bme280_temp_corr = root["bmet"] | sensors.bme280_temp_corr;
          sensors.bmp180_temp_corr = root["bmpt"] | sensors.bmp180_temp_corr;
          sensors.sht21_temp_corr = root["shtt"] | sensors.sht21_temp_corr;
          sensors.dht22_temp_corr = root["dhtt"] | sensors.dht22_temp_corr;
          sensors.bme280_hum_corr = root["bmeh"] | sensors.bme280_hum_corr;
          sensors.sht21_hum_corr = root["shth"] | sensors.sht21_hum_corr;
          sensors.dht22_hum_corr = root["dhth"] | sensors.dht22_hum_corr;
          sensors.bme280_pres_corr = root["bmep"] | sensors.bme280_pres_corr;
          sensors.bmp180_pres_corr = root["bmpp"] | sensors.bmp180_pres_corr;
          sensors.max44009_light_corr = root["max"] | sensors.max44009_light_corr;
          sensors.bh1750_light_corr = root["bh"] | sensors.bh1750_light_corr;

          config.wsens_temp_corr = root["wtempc"] | config.wsens_temp_corr;
          config.wsens_hum_corr = root["whumc"] | config.wsens_hum_corr;
          config.wsens_pres_corr = root["wpresc"] | config.wsens_pres_corr;
          config.wsens_light_corr = root["wlightc"] | config.wsens_light_corr;
          uint8_t k = root["wbatc"] | 300 - config.bat_k;
          config.bat_k = 300 - k;
          datas.bat_voltage = (float)datas.bat_adc / (float)config.bat_k;

          config.wsens_channel = root["chnum"] | config.wsens_channel;
          config.wexpire = root["wexpire"] | config.wexpire;
          config.bat_type = root["battype"] | config.bat_type;
          config.tupd = root["tupd"] | config.tupd;
          config.mqttsend = root["mqttsend"] | config.mqttsend;
          config.thngsend = root["thngsend"] | config.thngsend;
          config.nrdmsend = root["nrdmsend"] | config.nrdmsend;

          config.mqttrcv = root["mqttrcv"] | config.mqttrcv;
          config.thngrcv = root["thngrcv"] | config.thngrcv;

          config.nrd_tout = root["nrd_tout"] | config.nrd_tout;
          config.nrd_hout = root["nrd_hout"] | config.nrd_hout;
          config.nrd_pout = root["nrd_pout"] | config.nrd_pout;
          config.nrd_tin = root["nrd_tin"] | config.nrd_tin;
          config.nrd_hin = root["nrd_hin"] | config.nrd_hin;
          config.nrd_lin = root["nrd_lin"] | config.nrd_lin;

          strlcpy(config.mqttServer, root["mqttServer"] | config.mqttServer, sizeof(config.mqttServer));
          config.mqttPort = root["mqttPort"] | config.mqttPort;
          strlcpy(config.mqttLogin, root["mqttLogin"] | config.mqttLogin, sizeof(config.mqttLogin));
          strlcpy(config.mqttPass, root["mqttPass"] | config.mqttPass, sizeof(config.mqttPass));
          strlcpy(config.mqttF1, root["mqttF1"] | config.mqttF1, sizeof(config.mqttF1));
          strlcpy(config.mqttF2, root["mqttF2"] | config.mqttF2, sizeof(config.mqttF2));
          strlcpy(config.mqttF3, root["mqttF3"] | config.mqttF3, sizeof(config.mqttF3));
          strlcpy(config.mqttF4, root["mqttF4"] | config.mqttF4, sizeof(config.mqttF4));
          strlcpy(config.mqttF5, root["mqttF5"] | config.mqttF5, sizeof(config.mqttF5));
          strlcpy(config.mqttF6, root["mqttF6"] | config.mqttF6, sizeof(config.mqttF6));
          strlcpy(config.mqttF7, root["mqttF7"] | config.mqttF7, sizeof(config.mqttF7));
          strlcpy(config.mqttF8, root["mqttF8"] | config.mqttF8, sizeof(config.mqttF8));
          config.fq1 = root["fq1"] | config.fq1;
          config.fq2 = root["fq2"] | config.fq2;
          config.fq3 = root["fq3"] | config.fq3;
          config.fq4 = root["fq4"] | config.fq4;
          config.fq5 = root["fq5"] | config.fq5;
          config.fq6 = root["fq6"] | config.fq6;
          config.fq7 = root["fq7"] | config.fq7;
          config.fq8 = root["fq8"] | config.fq8;
          config.fd1[0] = root["fd1"][0] | config.fd1[0];
          config.fd2[0] = root["fd2"][0] | config.fd2[0];
          config.fd3[0] = root["fd3"][0] | config.fd3[0];
          config.fd4[0] = root["fd4"][0] | config.fd4[0];
          config.fd5[0] = root["fd5"][0] | config.fd5[0];
          config.fd6[0] = root["fd6"][0] | config.fd6[0];
          config.fd7[0] = root["fd7"][0] | config.fd7[0];
          config.fd8[0] = root["fd8"][0] | config.fd8[0];
          config.fd1[1] = root["fd1"][1] | config.fd1[1];
          config.fd2[1] = root["fd2"][1] | config.fd2[1];
          config.fd3[1] = root["fd3"][1] | config.fd3[1];
          config.fd4[1] = root["fd4"][1] | config.fd4[1];
          config.fd5[1] = root["fd5"][1] | config.fd5[1];
          config.fd6[1] = root["fd6"][1] | config.fd6[1];
          config.fd7[1] = root["fd7"][1] | config.fd7[1];
          config.fd8[1] = root["fd8"][1] | config.fd8[1];

          config.f1[0] = root["f1"][0] | config.f1[0];
          config.f2[0] = root["f2"][0] | config.f2[0];
          config.f3[0] = root["f3"][0] | config.f3[0];
          config.f4[0] = root["f4"][0] | config.f4[0];
          config.f5[0] = root["f5"][0] | config.f5[0];
          config.f6[0] = root["f6"][0] | config.f6[0];
          config.f7[0] = root["f7"][0] | config.f7[0];
          config.f8[0] = root["f8"][0] | config.f8[0];
          config.f1[1] = root["f1"][1] | config.f1[1];
          config.f2[1] = root["f2"][1] | config.f2[1];
          config.f3[1] = root["f3"][1] | config.f3[1];
          config.f4[1] = root["f4"][1] | config.f4[1];
          config.f5[1] = root["f5"][1] | config.f5[1];
          config.f6[1] = root["f6"][1] | config.f6[1];
          config.f7[1] = root["f7"][1] | config.f7[1];
          config.f8[1] = root["f8"][1] | config.f8[1];
          strlcpy(config.rdkey, root["rdkey"] | config.rdkey, sizeof(config.rdkey));
          strlcpy(config.wrkey, root["wrkey"] | config.wrkey, sizeof(config.wrkey));
          config.chid = root["chid"] | config.chid;

          config.mto = root["mto"] | config.mto;
          config.mho = root["mho"] | config.mho;
          config.mpo = root["mpo"] | config.mpo;
          config.mti = root["mti"] | config.mti;
          config.mhi = root["mhi"] | config.mhi;
          config.mli = root["mli"] | config.mli;

          config.tto = root["tto"] | config.tto;
          config.tho = root["tho"] | config.tho;
          config.tpo = root["tpo"] | config.tpo;
          config.tti = root["tti"] | config.tti;
          config.thi = root["thi"] | config.thi;
          config.tli = root["tli"] | config.tli;
          config.tbt = root["tbt"] | config.tbt;

          strlcpy(config.mqttT1, root["mqttT1"] | config.mqttT1, sizeof(config.mqttT1));
          strlcpy(config.mqttT2, root["mqttT2"] | config.mqttT2, sizeof(config.mqttT2));
          strlcpy(config.mqttT3, root["mqttT3"] | config.mqttT3, sizeof(config.mqttT3));
          strlcpy(config.mqttT4, root["mqttT4"] | config.mqttT4, sizeof(config.mqttT4));
          strlcpy(config.mqttT5, root["mqttT5"] | config.mqttT5, sizeof(config.mqttT5));
          strlcpy(config.mqttT6, root["mqttT6"] | config.mqttT6, sizeof(config.mqttT6));
          strlcpy(config.mqttT7, root["mqttT7"] | config.mqttT7, sizeof(config.mqttT7));
          strlcpy(config.mqttT8, root["mqttT8"] | config.mqttT8, sizeof(config.mqttT8));
          config.tq1 = root["tq1"] | config.tq1;
          config.tq2 = root["tq2"] | config.tq2;
          config.tq3 = root["tq3"] | config.tq3;
          config.tq4 = root["tq4"] | config.tq4;
          config.tq5 = root["tq5"] | config.tq5;
          config.tq6 = root["tq6"] | config.tq6;
          config.tq7 = root["tq7"] | config.tq7;
          config.tq8 = root["tq8"] | config.tq8;

          config.dp[0] = root["d0"][0] | config.dp[0];
          config.dp[1] = root["d1"][0] | config.dp[1];
          config.dp[2] = root["d2"][0] | config.dp[2];
          config.dp[3] = root["d3"][0] | config.dp[3];
          config.dp[4] = root["d4"][0] | config.dp[4];
          config.dp[5] = root["d5"][0] | config.dp[5];
          config.dt[0] = root["d0"][1] | config.dt[0];
          config.dt[1] = root["d1"][1] | config.dt[1];
          config.dt[2] = root["d2"][1] | config.dt[2];
          config.dt[3] = root["d3"][1] | config.dt[3];
          config.dt[4] = root["d4"][1] | config.dt[4];
          config.dt[5] = root["d5"][1] | config.dt[5];
          config.dc[0] = root["d0"][2] | config.dc[0];
          config.dc[1] = root["d1"][2] | config.dc[1];
          config.dc[2] = root["d2"][2] | config.dc[2];
          config.dc[3] = root["d3"][2] | config.dc[3];
          config.dc[4] = root["d4"][2] | config.dc[4];
          config.dc[5] = root["d5"][2] | config.dc[5];
          strlcpy(config.ds[0], root["d0"][3] | config.ds[0], sizeof(config.ds[0]));
          strlcpy(config.ds[1], root["d1"][3] | config.ds[1], sizeof(config.ds[1]));
          strlcpy(config.ds[2], root["d2"][3] | config.ds[2], sizeof(config.ds[2]));
          strlcpy(config.ds[3], root["d3"][3] | config.ds[3], sizeof(config.ds[3]));
          strlcpy(config.ds[4], root["d4"][3] | config.ds[4], sizeof(config.ds[4]));
          strlcpy(config.ds[5], root["d5"][3] | config.ds[5], sizeof(config.ds[5]));
                    
          save_config();
        }
      }
    }
  }
}

void TaskHC12rcv(void *pvParameters){
  (void) pvParameters;
  String wsensor = "";
  wsensor.reserve(255);
  while(1){
    while(Serial2.available()){
      wsensor = Serial2.readStringUntil('\n');
      Serial.print(wsensor);
      if(wsensor.lastIndexOf("{") != -1){
        StaticJsonDocument<1024> root;
        DeserializationError error = deserializeJson(root, wsensor);
        if(!error){
          datas.time_wsens = now();
          datas.temp_wsens = root["t"][0].as<float>() + config.wsens_temp_corr;
          uint8_t wtemp_sens = root["t"][1].as<uint8_t>();
          datas.hum_wsens = root["h"][0].as<float>() + config.wsens_hum_corr;
          uint8_t whum_sens = root["h"][1].as<uint8_t>();
          uint16_t prs = root["p"][0].as<float>() * 0.75 + config.wsens_pres_corr;
          if(prs > 500 and prs < 1200) datas.pres_wsens = prs;
          uint8_t wpres_sens = root["p"][1].as<uint8_t>();
          datas.light_wsens = root["l"][0].as<float>() + config.wsens_light_corr;
          uint8_t wlight_sens = root["l"][1].as<uint8_t>();
          datas.bat_adc = root["b"].as<int>();
          datas.wbat_voltage = (float)datas.bat_adc / (float)config.bat_k;
          String wrs = root["s"][wtemp_sens].as<String>();
          wrs.toCharArray(datas.wtemp_sens, wrs.length() + 1);
          wrs = root["s"][whum_sens].as<String>();
          wrs.toCharArray(datas.whum_sens, wrs.length() + 1);
          wrs = root["s"][wpres_sens].as<String>();
          wrs.toCharArray(datas.wpres_sens, wrs.length() + 1);
          wrs = root["s"][wlight_sens].as<String>();
          wrs.toCharArray(datas.wlight_sens, wrs.length() + 1);

          Serial.print("datas.time_wsens=");
          Serial.print(hour(datas.time_wsens)); Serial.print(":");
          Serial.print(minute(datas.time_wsens)); Serial.print(":");
          Serial.println(second(datas.time_wsens));
          Serial.print("datas.temp_wsens="); Serial.println(datas.temp_wsens);
          Serial.print("datas.hum_wsens="); Serial.println(datas.hum_wsens);
          Serial.print("datas.pres_wsens="); Serial.println(datas.pres_wsens);
          Serial.print("datas.light_wsens="); Serial.println(datas.light_wsens);
          Serial.print("datas.bat_voltage="); Serial.println(datas.wbat_voltage);
          Serial.print("datas.wtemp_wsens="); Serial.println(datas.temp_wsens);
          Serial.print("datas.whum_sens="); Serial.println(datas.whum_sens);
          Serial.print("datas.wpres_sens="); Serial.println(datas.wpres_sens);
          Serial.print("datas.wlight_sens="); Serial.println(datas.wlight_sens);

          float umin = 3.3;
          float umax = 5.0;
          if(config.bat_type == 0){
            umin = 3.5;
            umax = 4.5;
          }
          if(config.bat_type == 1){
            umin = 3.5;
            umax = 3.9;
          }
          float stp = (umax - umin) / 4;
          if(datas.wbat_voltage < (umin + stp)) datas.wbat_level = 1;
          else if(datas.wbat_voltage < (umin + stp * 2)) datas.wbat_level = 2;
          else if(datas.wbat_voltage < (umin + stp * 3)) datas.wbat_level = 3;
          else datas.wbat_level = 4;
        }
      }
      if(wsensor.lastIndexOf("OK+RC") != -1){
        uint8_t ch = wsensor.substring(5).toInt();
        if(ch != config.wsens_channel){
          Serial2.printf("AT+C%03d\r\n", config.wsens_channel);
          delay(100);
          digitalWrite(SET_HC12, HIGH);
        }
        else digitalWrite(SET_HC12, HIGH);
      }
    }
    vTaskDelay(1);
    wsensor = "";
  }
}

void scan_nets(void){
  String json = "{";
  uint8_t n = WiFi.scanNetworks();
  for(uint8_t i = 0; i < n; i++){
    json += "\"";
    json += WiFi.SSID(i);
    json += "\":\"";
    json += abs(WiFi.RSSI(i));
    if(i == n - 1) json += "\"}";
    else json += "\",";
  }
  json.toCharArray(datas.networks, (json.length()) + 1);
  datas.ant = WiFi.RSSI();
}

boolean is_summertime(){
  if(month() < 3 || month() > 10) return false;
  if(month() > 3 && month() < 10) return true;
  if((month() == 3 && (hour() + 24 * day()) >= (1 + 24 * (31 - (5 * year() / 4 + 4) % 7))) || (month() == 10 && (hour() + 24 * day()) < (1 + 24 * (31 - (5 * year() / 4 + 1) % 7)))) return true;
  else return false;
}

time_t getTime(void){
  uint16_t daylight = 0;
  if(datas.clock_synchronized) if(is_summertime()) daylight = config.daylight * 3600;
  configTime(config.utc * 3600, daylight, config.ntp, "0.pool.ntp.org", "1.pool.ntp.org");
  struct tm tmstruct;
  vTaskDelay(2000);
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct, 5000);
  setTime(tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec, tmstruct.tm_mday, tmstruct.tm_mon + 1, tmstruct.tm_year + 1900);
  datas.clock_synchronized = true;
  return now();
}

void save_config(void){
  String json;
  DynamicJsonDocument conf(4096);
  conf["ssid"] = config.ssid;
  conf["pass"] = config.pass;
  conf["mask"] = config.mask;
  conf["dns1"] = config.dns1;
  conf["dns2"] = config.dns2;
  conf["ip"] = config.ip;
  conf["gw"] = config.gw;
  conf["type"] = config.type;

  conf["lang"] = config.lang;
  conf["brt"] = config.brt;
  conf["brday"] = config.brday;
  conf["brnight"] = config.brnight;
  conf["hd"] = config.hd;
  conf["md"] = config.md;
  conf["hn"] = config.hn;
  conf["mn"] = config.mn;

  conf["temp_out"] = config.temp_out;
  conf["hum_out"] = config.hum_out;
  conf["pres_out"] = config.pres_out;
  conf["temp_in"] = config.temp_in;
  conf["hum_in"] = config.hum_in;
  conf["light_in"] = config.light_in;
  conf["bat_disp"] = config.bat_disp;

  conf["utc"] = config.utc;
  conf["dlst"] = config.daylight;
  conf["ntp"] = config.ntp;

  conf["city"] = config.city;
  conf["cityid"] = config.cityid;
  conf["lat"] = config.lat;
  conf["lon"] = config.lon;
  conf["appid"] = config.appid;
  conf["citysearch"] = config.citysearch;

  conf["bmet"] = sensors.bme280_temp_corr;
  conf["bmpt"] = sensors.bmp180_temp_corr;
  conf["shtt"] = sensors.sht21_temp_corr;
  conf["dhtt"] = sensors.dht22_temp_corr;
  conf["bmeh"] = sensors.bme280_hum_corr;
  conf["shth"] = sensors.sht21_hum_corr;
  conf["dhth"] = sensors.dht22_hum_corr;
  conf["bmep"] = sensors.bme280_pres_corr;
  conf["bmpp"] = sensors.bmp180_pres_corr;
  conf["max"] = sensors.max44009_light_corr;
  conf["bh"] = sensors.bh1750_light_corr;

  conf["wtempc"] = config.wsens_temp_corr;
  conf["whumc"] = config.wsens_hum_corr;
  conf["wpresc"] = config.wsens_pres_corr;
  conf["wlightc"] = config.wsens_light_corr;
  conf["wbatc"] = config.bat_k;
  conf["chnum"] = config.wsens_channel;
  conf["wexpire"] = config.wexpire;
  conf["battype"] = config.bat_type;

  conf["tupd"] = config.tupd;
  conf["mqttsend"] = config.mqttsend;
  conf["thngsend"] = config.thngsend;
  conf["nrdmsend"] = config.nrdmsend;

  conf["mqttrcv"] = config.mqttrcv;
  conf["thngrcv"] = config.thngrcv;

  conf["nrd_tout"] = config.nrd_tout;
  conf["nrd_hout"] = config.nrd_hout;
  conf["nrd_pout"] = config.nrd_pout;
  conf["nrd_tin"] = config.nrd_tin;
  conf["nrd_hin"] = config.nrd_hin;
  conf["nrd_lin"] = config.nrd_lin;

  conf["mqttServer"] = config.mqttServer;
  conf["mqttPort"] = config.mqttPort;
  String str = String(config.mqttLogin);
  str.replace("-", "_");
  conf["mqttLogin"] = str;
  str = String(config.mqttPass);
  str.replace("-", "_");
  conf["mqttPass"] = str;
  conf["fq1"] = config.fq1;
  conf["fq2"] = config.fq2;
  conf["fq3"] = config.fq3;
  conf["fq4"] = config.fq4;
  conf["fq5"] = config.fq5;
  conf["fq6"] = config.fq6;
  conf["fq7"] = config.fq7;
  conf["fq8"] = config.fq8;
  conf["mqttF1"] = config.mqttF1;
  conf["mqttF2"] = config.mqttF2;
  conf["mqttF3"] = config.mqttF3;
  conf["mqttF4"] = config.mqttF4;
  conf["mqttF5"] = config.mqttF5;
  conf["mqttF6"] = config.mqttF6;
  conf["mqttF7"] = config.mqttF7;
  conf["mqttF8"] = config.mqttF8;
  JsonArray fd1 = conf.createNestedArray("fd1");
  fd1.add(config.fd1[0]);
  fd1.add(config.fd1[1]);
  JsonArray fd2 = conf.createNestedArray("fd2");
  fd2.add(config.fd2[0]);
  fd2.add(config.fd2[1]);
  JsonArray fd3 = conf.createNestedArray("fd3");
  fd3.add(config.fd3[0]);
  fd3.add(config.fd3[1]);
  JsonArray fd4 = conf.createNestedArray("fd4");
  fd4.add(config.fd4[0]);
  fd4.add(config.fd4[1]);
  JsonArray fd5 = conf.createNestedArray("fd5");
  fd5.add(config.fd5[0]);
  fd5.add(config.fd5[1]);
  JsonArray fd6 = conf.createNestedArray("fd6");
  fd6.add(config.fd6[0]);
  fd6.add(config.fd6[1]);
  JsonArray fd7 = conf.createNestedArray("fd7");
  fd7.add(config.fd7[0]);
  fd7.add(config.fd7[1]);
  JsonArray fd8 = conf.createNestedArray("fd8");
  fd8.add(config.fd8[0]);
  fd8.add(config.fd8[1]);

  JsonArray f1 = conf.createNestedArray("f1");
  f1.add(config.f1[0]);
  f1.add(config.f1[1]);
  JsonArray f2 = conf.createNestedArray("f2");
  f2.add(config.f2[0]);
  f2.add(config.f2[1]);
  JsonArray f3 = conf.createNestedArray("f3");
  f3.add(config.f3[0]);
  f3.add(config.f3[1]);
  JsonArray f4 = conf.createNestedArray("f4");
  f4.add(config.f4[0]);
  f4.add(config.f4[1]);
  JsonArray f5 = conf.createNestedArray("f5");
  f5.add(config.f5[0]);
  f5.add(config.f5[1]);
  JsonArray f6 = conf.createNestedArray("f6");
  f6.add(config.f6[0]);
  f6.add(config.f6[1]);
  JsonArray f7 = conf.createNestedArray("f7");
  f7.add(config.f7[0]);
  f7.add(config.f7[1]);
  JsonArray f8 = conf.createNestedArray("f8");
  f8.add(config.f8[0]);
  f8.add(config.f8[1]);
  conf["rdkey"] = config.rdkey;
  conf["wrkey"] = config.wrkey;
  conf["chid"] = config.chid;

  conf["mto"] = config.mto;
  conf["mho"] = config.mho;
  conf["mpo"] = config.mpo;
  conf["mti"] = config.mti;
  conf["mhi"] = config.mhi;
  conf["mli"] = config.mli;

  conf["tto"] = config.tto;
  conf["tho"] = config.tho;
  conf["tpo"] = config.tpo;
  conf["tti"] = config.tti;
  conf["thi"] = config.thi;
  conf["tli"] = config.tli;
  conf["tbt"] = config.tbt;

  conf["tq1"] = config.tq1;
  conf["tq2"] = config.tq2;
  conf["tq3"] = config.tq3;
  conf["tq4"] = config.tq4;
  conf["tq5"] = config.tq5;
  conf["tq6"] = config.tq6;
  conf["tq7"] = config.tq7;
  conf["tq8"] = config.tq8;
  conf["mqttT1"] = config.mqttT1;
  conf["mqttT2"] = config.mqttT2;
  conf["mqttT3"] = config.mqttT3;
  conf["mqttT4"] = config.mqttT4;
  conf["mqttT5"] = config.mqttT5;
  conf["mqttT6"] = config.mqttT6;
  conf["mqttT7"] = config.mqttT7;
  conf["mqttT8"] = config.mqttT8;

  conf["dp0"] = config.dp[0];
  conf["dp1"] = config.dp[1];
  conf["dp2"] = config.dp[2];
  conf["dp3"] = config.dp[3];
  conf["dp4"] = config.dp[4];
  conf["dp5"] = config.dp[5];
  conf["dt0"] = config.dt[0];
  conf["dt1"] = config.dt[1];
  conf["dt2"] = config.dt[2];
  conf["dt3"] = config.dt[3];
  conf["dt4"] = config.dt[4];
  conf["dt5"] = config.dt[5];
  conf["dc0"] = config.dc[0];
  conf["dc1"] = config.dc[1];
  conf["dc2"] = config.dc[2];
  conf["dc3"] = config.dc[3];
  conf["dc4"] = config.dc[4];
  conf["dc5"] = config.dc[5];
  conf["ds0"] = config.ds[0];
  conf["ds1"] = config.ds[1];
  conf["ds2"] = config.ds[2];
  conf["ds3"] = config.ds[3];
  conf["ds4"] = config.ds[4];
  conf["ds5"] = config.ds[5];

  serializeJson(conf, json);
  File file = SPIFFS.open("/config.json", FILE_WRITE);
  if(file.print(json)) myNex.writeStr("vis ok,0");
  else Serial.println("ERROR Write file");
  file.close();
  Serial.println(json);
}

void read_config(void){
  File file = SPIFFS.open("/config.json");
  while(file.available()){
    String json = file.readString();
    Serial.println(json);
    DynamicJsonDocument conf(8192);
    //StaticJsonDocument<4096> conf;
    DeserializationError error = deserializeJson(conf, json);
    if(!error){
      strlcpy(config.lang, conf["lang"], sizeof(config.lang));
      strlcpy(config.ssid, conf["ssid"], sizeof(config.ssid));
      strlcpy(config.pass, conf["pass"], sizeof(config.pass));
      strlcpy(config.mask, conf["mask"], sizeof(config.mask));
      strlcpy(config.dns1, conf["dns1"], sizeof(config.dns1));
      strlcpy(config.dns2, conf["dns2"], sizeof(config.dns2));
      strlcpy(config.ip, conf["ip"], sizeof(config.ip));
      strlcpy(config.gw, conf["gw"], sizeof(config.gw));
      config.type = conf["type"].as<bool>();

      config.brt = conf["brt"];
      config.brday = conf["brday"];
      config.brnight = conf["brnight"];
      config.hd = conf["hd"];
      config.md = conf["md"];
      config.hn = conf["hn"];
      config.mn = conf["mn"];

      config.temp_out = conf["temp_out"];
      config.hum_out = conf["hum_out"];
      config.pres_out = conf["pres_out"];
      config.temp_in = conf["temp_in"];
      config.hum_in = conf["hum_in"];
      config.light_in = conf["light_in"];
      config.bat_disp = conf["bat_disp"];

      config.utc = conf["utc"];
      config.daylight = conf["dlst"].as<bool>();
      strlcpy(config.ntp, conf["ntp"], sizeof(config.ntp));

      strlcpy(config.city, conf["city"], sizeof(config.city));
      strlcpy(config.cityid, conf["cityid"], sizeof(config.cityid));
      strlcpy(config.lat, conf["lat"], sizeof(config.lat));
      strlcpy(config.lon, conf["lon"], sizeof(config.lon));
      strlcpy(config.appid, conf["appid"], sizeof(config.appid));
      config.citysearch = conf["citysearch"];

      sensors.bme280_temp_corr = conf["bmet"];
      sensors.bmp180_temp_corr = conf["bmpt"];
      sensors.sht21_temp_corr = conf["shtt"];
      sensors.dht22_temp_corr = conf["dhtt"];
      sensors.bme280_hum_corr = conf["bmeh"];
      sensors.sht21_hum_corr = conf["shth"];
      sensors.dht22_hum_corr = conf["dhth"];
      sensors.bme280_pres_corr = conf["bmep"];
      sensors.bmp180_pres_corr = conf["bmpp"];
      sensors.max44009_light_corr = conf["max"];
      sensors.bh1750_light_corr = conf["bh"];

      config.wsens_temp_corr = conf["wtempc"];
      config.wsens_hum_corr = conf["whumc"];
      config.wsens_pres_corr = conf["wpresc"];
      config.wsens_light_corr = conf["wlightc"];
      config.bat_k = conf["wbatc"];
      config.wsens_channel = conf["chnum"];
      config.wexpire = conf["wexpire"];
      config.bat_type = conf["battype"];

      config.tupd = conf["tupd"];
      config.mqttsend = conf["mqttsend"];
      config.thngsend = conf["thngsend"];
      config.nrdmsend = conf["nrdmsend"];

      config.mqttrcv = conf["mqttrcv"];
      config.thngrcv = conf["thngrcv"];

      config.nrd_tout = conf["nrd_tout"];
      config.nrd_hout = conf["nrd_hout"];
      config.nrd_pout = conf["nrd_pout"];
      config.nrd_tin = conf["nrd_tin"];
      config.nrd_hin = conf["nrd_hin"];
      config.nrd_lin = conf["nrd_lin"];

      strlcpy(config.mqttServer, conf["mqttServer"], sizeof(config.mqttServer));
      config.mqttPort = conf["mqttPort"];
      strlcpy(config.mqttLogin, conf["mqttLogin"], sizeof(config.mqttLogin));
      strlcpy(config.mqttPass, conf["mqttPass"], sizeof(config.mqttPass));
      strlcpy(config.mqttF1, conf["mqttF1"], sizeof(config.mqttF1));
      strlcpy(config.mqttF2, conf["mqttF2"], sizeof(config.mqttF2));
      strlcpy(config.mqttF3, conf["mqttF3"], sizeof(config.mqttF3));
      strlcpy(config.mqttF4, conf["mqttF4"], sizeof(config.mqttF4));
      strlcpy(config.mqttF5, conf["mqttF5"], sizeof(config.mqttF5));
      strlcpy(config.mqttF6, conf["mqttF6"], sizeof(config.mqttF6));
      strlcpy(config.mqttF7, conf["mqttF7"], sizeof(config.mqttF7));
      strlcpy(config.mqttF8, conf["mqttF8"], sizeof(config.mqttF8));
      config.fq1 = conf["fq1"];
      config.fq2 = conf["fq2"];
      config.fq3 = conf["fq3"];
      config.fq4 = conf["fq4"];
      config.fq5 = conf["fq5"];
      config.fq6 = conf["fq6"];
      config.fq7 = conf["fq7"];
      config.fq8 = conf["fq8"];
      config.fd1[0] = conf["fd1"][0];
      config.fd1[1] = conf["fd1"][1];
      config.fd2[0] = conf["fd2"][0];
      config.fd2[1] = conf["fd2"][1];
      config.fd3[0] = conf["fd3"][0];
      config.fd3[1] = conf["fd3"][1];
      config.fd4[0] = conf["fd4"][0];
      config.fd4[1] = conf["fd4"][1];
      config.fd5[0] = conf["fd5"][0];
      config.fd5[1] = conf["fd5"][1];
      config.fd6[0] = conf["fd6"][0];
      config.fd6[1] = conf["fd6"][1];
      config.fd7[0] = conf["fd7"][0];
      config.fd7[1] = conf["fd7"][1];
      config.fd8[0] = conf["fd8"][0];
      config.fd8[1] = conf["fd8"][1];

      config.f1[0] = conf["f1"][0];
      config.f1[1] = conf["f1"][1];
      config.f2[0] = conf["f2"][0];
      config.f2[1] = conf["f2"][1];
      config.f3[0] = conf["f3"][0];
      config.f3[1] = conf["f3"][1];
      config.f4[0] = conf["f4"][0];
      config.f4[1] = conf["f4"][1];
      config.f5[0] = conf["f5"][0];
      config.f5[1] = conf["f5"][1];
      config.f6[0] = conf["f6"][0];
      config.f6[1] = conf["f6"][1];
      config.f7[0] = conf["f7"][0];
      config.f7[1] = conf["f7"][1];
      config.f8[0] = conf["f8"][0];
      config.f8[1] = conf["f8"][1];
      strlcpy(config.rdkey, conf["rdkey"], sizeof(config.rdkey));
      strlcpy(config.wrkey, conf["wrkey"], sizeof(config.wrkey));
      config.chid = conf["chid"];

      config.mto = conf["mto"];
      config.mho = conf["mho"];
      config.mpo = conf["mpo"];
      config.mti = conf["mti"];
      config.mhi = conf["mhi"];
      config.mli = conf["mli"];

      config.tto = conf["tto"];
      config.tho = conf["tho"];
      config.tpo = conf["tpo"];
      config.tti = conf["tti"];
      config.thi = conf["thi"];
      config.tli = conf["tli"];
      config.tbt = conf["tbt"];

      strlcpy(config.mqttT1, conf["mqttT1"], sizeof(config.mqttT1));
      strlcpy(config.mqttT2, conf["mqttT2"], sizeof(config.mqttT2));
      strlcpy(config.mqttT3, conf["mqttT3"], sizeof(config.mqttT3));
      strlcpy(config.mqttT4, conf["mqttT4"], sizeof(config.mqttT4));
      strlcpy(config.mqttT5, conf["mqttT5"], sizeof(config.mqttT5));
      strlcpy(config.mqttT6, conf["mqttT6"], sizeof(config.mqttT6));
      strlcpy(config.mqttT7, conf["mqttT7"], sizeof(config.mqttT7));
      strlcpy(config.mqttT8, conf["mqttT8"], sizeof(config.mqttT8));
      config.tq1 = conf["tq1"];
      config.tq2 = conf["tq2"];
      config.tq3 = conf["tq3"];
      config.tq4 = conf["tq4"];
      config.tq5 = conf["tq5"];
      config.tq6 = conf["tq6"];
      config.tq7 = conf["tq7"];
      config.tq8 = conf["tq8"];

      config.dp[0] = conf["dp0"];
      config.dp[1] = conf["dp1"];
      config.dp[2] = conf["dp2"];
      config.dp[3] = conf["dp3"];
      config.dp[4] = conf["dp4"];
      config.dp[5] = conf["dp5"];
      config.dt[0] = conf["dt0"];
      config.dt[1] = conf["dt1"];
      config.dt[2] = conf["dt2"];
      config.dt[3] = conf["dt3"];
      config.dt[4] = conf["dt4"];
      config.dt[5] = conf["dt5"];
      config.dc[0] = conf["dc0"];
      config.dc[1] = conf["dc1"];
      config.dc[2] = conf["dc2"];
      config.dc[3] = conf["dc3"];
      config.dc[4] = conf["dc4"];
      config.dc[5] = conf["dc5"];
      strlcpy(config.ds[0], conf["ds0"], sizeof(config.ds[0]));
      strlcpy(config.ds[1], conf["ds1"], sizeof(config.ds[1]));
      strlcpy(config.ds[2], conf["ds2"], sizeof(config.ds[2]));
      strlcpy(config.ds[3], conf["ds3"], sizeof(config.ds[3]));
      strlcpy(config.ds[4], conf["ds4"], sizeof(config.ds[4]));
      strlcpy(config.ds[5], conf["ds5"], sizeof(config.ds[5]));
      
      digitalWrite(SET_HC12, LOW);
      delay(45);
      Serial2.println("AT+RC");

      Serial.print("config.lang="); Serial.println(config.lang);
      Serial.print("config.ssid="); Serial.println(config.ssid);
      Serial.print("config.pass="); Serial.println(config.pass);
      Serial.print("config.mask="); Serial.println(config.mask);
      Serial.print("config.dns1="); Serial.println(config.dns1);
      Serial.print("config.dns2="); Serial.println(config.dns2);
      Serial.print("config.ip="); Serial.println(config.ip);
      Serial.print("config.gw="); Serial.println(config.gw);
      Serial.print("config.type="); Serial.println(config.type);
      Serial.println();
      Serial.print("config.brt="); Serial.println(config.brt);
      Serial.print("config.brday="); Serial.println(config.brday);
      Serial.print("config.brnight="); Serial.println(config.brnight);
      Serial.print("config.hd="); Serial.println(config.hd);
      Serial.print("config.md="); Serial.println(config.md);
      Serial.print("config.hn="); Serial.println(config.hn);
      Serial.print("config.mn="); Serial.println(config.mn);
      Serial.println();
      Serial.print("config.temp_out="); Serial.println(config.temp_out);
      Serial.print("config.hum_out="); Serial.println(config.hum_out);
      Serial.print("config.pres_out="); Serial.println(config.pres_out);
      Serial.print("config.temp_in="); Serial.println(config.temp_in);
      Serial.print("config.hum_in="); Serial.println(config.hum_in);
      Serial.print("config.light_in="); Serial.println(config.light_in);
      Serial.println();
      Serial.print("config.utc="); Serial.println(config.utc);
      Serial.print("config.daylight="); Serial.println(config.daylight);
      Serial.print("config.ntp="); Serial.println(config.ntp);
      Serial.println();
      Serial.print("config.city="); Serial.println(config.city);
      Serial.print("config.cityid="); Serial.println(config.cityid);
      Serial.print("config.lat="); Serial.println(config.lat);
      Serial.print("config.lon="); Serial.println(config.lon);
      Serial.print("config.appid="); Serial.println(config.appid);
      Serial.print("config.citysearch="); Serial.println(config.citysearch);
      Serial.println();
      Serial.print("sensors.bme280_temp_corr="); Serial.println(sensors.bme280_temp_corr);
      Serial.print("sensors.bmp180_temp_corr="); Serial.println(sensors.bmp180_temp_corr);
      Serial.print("sensors.sht21_temp_corr="); Serial.println(sensors.sht21_temp_corr);
      Serial.print("sensors.dht22_temp_corr="); Serial.println(sensors.dht22_temp_corr);
      Serial.print("sensors.bme280_hum_corr="); Serial.println(sensors.bme280_hum_corr);
      Serial.print("sensors.sht21_hum_corr="); Serial.println(sensors.sht21_hum_corr);
      Serial.print("sensors.dht22_hum_corr="); Serial.println(sensors.dht22_hum_corr);
      Serial.print("sensors.bme280_pres_corr="); Serial.println(sensors.bme280_pres_corr);
      Serial.print("sensors.bmp180_pres_corr="); Serial.println(sensors.bmp180_pres_corr);
      Serial.print("sensors.max44009_light_corr="); Serial.println(sensors.max44009_light_corr);
      Serial.print("sensors.bh1750_light_corr="); Serial.println(sensors.bh1750_light_corr);
      Serial.println();
      Serial.print("config.wsens_temp_corr="); Serial.println(config.wsens_temp_corr);
      Serial.print("config.wsens_hum_corr="); Serial.println(config.wsens_hum_corr);
      Serial.print("config.wsens_pres_corr="); Serial.println(config.wsens_pres_corr);
      Serial.print("config.wsens_light_corr="); Serial.println(config.wsens_light_corr);
      Serial.print("config.bat_k="); Serial.println(config.bat_k);
      Serial.print("config.wsens_channel="); Serial.println(config.wsens_channel);
      Serial.print("config.wexpire="); Serial.println(config.wexpire);
      Serial.print("config.bat_type="); Serial.println(config.bat_type);
      Serial.println();
      Serial.print("config.tupd="); Serial.println(config.tupd);
      Serial.print("config.mqttsend="); Serial.println(config.mqttsend);
      Serial.print("config.thngsend="); Serial.println(config.thngsend);
      Serial.print("config.nrdmsend="); Serial.println(config.nrdmsend);
      Serial.println();
      Serial.print("config.nrd_tout="); Serial.println(config.nrd_tout);
      Serial.print("config.nrd_hout="); Serial.println(config.nrd_hout);
      Serial.print("config.nrd_pout="); Serial.println(config.nrd_pout);
      Serial.print("config.nrd_tin="); Serial.println(config.nrd_tin);
      Serial.print("config.nrd_hin="); Serial.println(config.nrd_hin);
      Serial.print("config.nrd_lin="); Serial.println(config.nrd_lin);
      Serial.println();
      Serial.print("config.f1[0]="); Serial.println(config.f1[0]);
      Serial.print("config.f1[1]="); Serial.println(config.f1[1]);
      Serial.print("config.f2[0]="); Serial.println(config.f2[0]);
      Serial.print("config.f2[1]="); Serial.println(config.f2[1]);
      Serial.print("config.f3[0]="); Serial.println(config.f3[0]);
      Serial.print("config.f3[1]="); Serial.println(config.f3[1]);
      Serial.print("config.f4[0]="); Serial.println(config.f4[0]);
      Serial.print("config.f4[1]="); Serial.println(config.f4[1]);
      Serial.print("config.f5[0]="); Serial.println(config.f5[0]);
      Serial.print("config.f5[1]="); Serial.println(config.f5[1]);
      Serial.print("config.f6[0]="); Serial.println(config.f6[0]);
      Serial.print("config.f6[1]="); Serial.println(config.f6[1]);
      Serial.print("config.f7[0]="); Serial.println(config.f7[0]);
      Serial.print("config.f7[1]="); Serial.println(config.f7[1]);
      Serial.print("config.f8[0]="); Serial.println(config.f8[0]);
      Serial.print("config.f8[1]="); Serial.println(config.f8[1]);
      Serial.print("config.rdkey="); Serial.println(config.rdkey);
      Serial.print("config.wrkey="); Serial.println(config.wrkey);
      Serial.print("config.chid="); Serial.println(config.chid);
      Serial.println();
      Serial.print("config.dp[0]="); Serial.println(config.dp[0]);
      Serial.print("config.dp[1]="); Serial.println(config.dp[1]);
      Serial.print("config.dp[2]="); Serial.println(config.dp[2]);
      Serial.print("config.dp[3]="); Serial.println(config.dp[3]);
      Serial.print("config.dp[4]="); Serial.println(config.dp[4]);
      Serial.print("config.dp[5]="); Serial.println(config.dp[5]);
      Serial.print("config.dt[0]="); Serial.println(config.dt[0]);
      Serial.print("config.dt[1]="); Serial.println(config.dt[1]);
      Serial.print("config.dt[2]="); Serial.println(config.dt[2]);
      Serial.print("config.dt[3]="); Serial.println(config.dt[3]);
      Serial.print("config.dt[4]="); Serial.println(config.dt[4]);
      Serial.print("config.dt[5]="); Serial.println(config.dt[5]);
      Serial.print("config.dc[0]="); Serial.println(config.dc[0]);
      Serial.print("config.dc[1]="); Serial.println(config.dc[1]);
      Serial.print("config.dc[2]="); Serial.println(config.dc[2]);
      Serial.print("config.dc[3]="); Serial.println(config.dc[3]);
      Serial.print("config.dc[4]="); Serial.println(config.dc[4]);
      Serial.print("config.dc[5]="); Serial.println(config.dc[5]);
      Serial.print("config.ds[0]="); Serial.println(config.ds[0]);
      Serial.print("config.ds[1]="); Serial.println(config.ds[1]);
      Serial.print("config.ds[2]="); Serial.println(config.ds[2]);
      Serial.print("config.ds[3]="); Serial.println(config.ds[3]);
      Serial.print("config.ds[4]="); Serial.println(config.ds[4]);
      Serial.print("config.ds[5]="); Serial.println(config.ds[5]);
      Serial.print("config.tto="); Serial.println(config.tto);
      Serial.print("config.tho="); Serial.println(config.tho);
      Serial.print("config.tpo="); Serial.println(config.tpo);
      Serial.print("config.tti="); Serial.println(config.tti);
      Serial.print("config.thi="); Serial.println(config.thi);
      Serial.print("config.tli="); Serial.println(config.tli);
      Serial.print("config.tbt="); Serial.println(config.tbt);
      Serial.println();
    }
    else Serial.println("deserialisation error");
  }
}
