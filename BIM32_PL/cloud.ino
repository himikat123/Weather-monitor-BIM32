void narodmon_send(void){
  float temp_out = 404.0;
  float temp_in = 404.0;
  float hum_out = 202.0;
  float hum_in = 202.0;
  float pres = 4040.0;
  float light = -1.0;
  String mac = WiFi.macAddress();
  mac.replace(":", "");
  String buf = "#BIM" + mac + "#BIM";
  buf += "#" + String(config.lat);
  buf += "#" + String(config.lon);
  switch(config.nrd_tout){
    case 0: temp_out = datas.temp_web; break;
    case 1: temp_out=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:404.0; break;
    case 2: temp_out = sensors.bme280_temp; break;
    case 3: temp_out = sensors.bmp180_temp; break;
    case 4: temp_out = sensors.sht21_temp; break;
    case 5: temp_out = sensors.dht22_temp; break;
    default: temp_out = 404.0; break;
  }
  switch(config.nrd_hout){
    case 0: hum_out = datas.hum_web; break;
    case 1: hum_out = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:202.0; break;
    case 2: hum_out = sensors.bme280_hum; break;
    case 4: hum_out = sensors.sht21_hum; break;
    case 5: hum_out = sensors.dht22_hum; break;
    default: hum_out = 202.0; break;
  }
  switch(config.nrd_pout){
    case 0: pres = datas.pres_web; break;
    case 1: pres = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0; break;
    case 2: pres = sensors.bme280_pres; break;
    case 3: pres = sensors.bmp180_pres; break;
    default: pres = 4040.0; break;
  }
  switch(config.nrd_tin){
    case 0: temp_in = datas.temp_web; break;
    case 1: temp_in = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:404.0; break;
    case 2: temp_in = sensors.bme280_temp; break;
    case 3: temp_in = sensors.bmp180_temp; break;
    case 4: temp_in = sensors.sht21_temp; break;
    case 5: temp_in = sensors.dht22_temp; break;
    default: temp_in = 404.0; break;
  }
  switch(config.nrd_hin){
    case 0: hum_in = datas.hum_web; break;
    case 1: hum_in = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:202.0; break;
    case 2: hum_in = sensors.bme280_hum; break;
    case 4: hum_in = sensors.sht21_hum; break;
    case 5: hum_in = sensors.dht22_hum; break;
    default: hum_in = 202.0; break;
  }
  switch(config.nrd_lin){
    case 1: light = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:-1.0; break;
    case 8: light = sensors.max44009_light; break;
    case 9: light = sensors.bh1750_light; break;
    default: light = -1.0; break;
  }
  if(temp_out > -55.0 and temp_out < 125.0) buf += "\n#T" + mac + "#" + String(temp_out);
  if(hum_out > -1.0 and hum_out < 101.0) buf += "\n#H" + mac + "#" + String(hum_out);
  if(pres > 569.0 and pres < 1101.0) buf += "\n#P" + mac + "#" + String(pres);
  if(temp_in > -55.0 and temp_in < 125.0) buf += "\n#TI" + mac + "#" + String(temp_in);
  if(hum_in > -1.0 and hum_in < 101.0) buf += "\n#HI" + mac + "#" + String(hum_in);
  if(light >= 0.0 and light < 200000.0) buf += "\n#L" + mac + "#" + String(light);
  if(datas.wbat_voltage >= 1.0) buf += "\n#B" + mac + "#" + String(datas.wbat_voltage);
  buf += "\n##\r\n";
  if(!client.connect("narodmon.ru", 8283)) Serial.println("send to narodmon failed");
  else client.print(buf);
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.printf("Server returned %s\r\n", line);
  }
  Serial.println(buf);
  Serial.println();
  client.stop();
}

void thingspk_recv(void){
  String url;
  url = "http://api.thingspeak.com/channels/";
  url += String(config.chid); 
  url += "/feeds.json?api_key=";
  url += String(config.rdkey);
  url += "&results=1";
  parseThing(thingRequest(url));
}

String thingRequest(String url){
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

void parseThing(String httpData){
  float thinh_fields[9] = {404.0, 404.0, 404.0, 404.0, 404.0, 404.0, 404.0, 404.0, 404.0};
  StaticJsonDocument<2048> root;
  DeserializationError error = deserializeJson(root, httpData);
  if(error) return;
  thinh_fields[1] = root["feeds"][0]["field1"];
  thinh_fields[2] = root["feeds"][0]["field2"];
  thinh_fields[3] = root["feeds"][0]["field3"];
  thinh_fields[4] = root["feeds"][0]["field4"];
  thinh_fields[5] = root["feeds"][0]["field5"];
  thinh_fields[6] = root["feeds"][0]["field6"];
  thinh_fields[7] = root["feeds"][0]["field7"];
  thinh_fields[8] = root["feeds"][0]["field8"];
  httpData = "";
  datas.thng_temp_out = thinh_fields[config.tto];
  datas.thng_temp_in = thinh_fields[config.tti];
  datas.thng_hum_out = thinh_fields[config.tho];
  datas.thng_hum_in = thinh_fields[config.thi];
  datas.thng_pres_out = thinh_fields[config.tpo];
  datas.thng_light_in = thinh_fields[config.tli];
  datas.thng_bat_disp = thinh_fields[config.tbt];
}

void thingspk_send(void){
  Serial.println("thingspeak send");
  String url;
  url = "http://api.thingspeak.com/update?api_key=";
  url += String(config.wrkey);
  float val = 404.0;
  if(config.f1[0] == 0){
    val = 404.0;
    if(config.f1[1] == 0) val = datas.temp_web;
    else if(config.f1[1] == 1) val = datas.hum_web;
    else if(config.f1[1] == 2) val = datas.pres_web;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field1="; 
      url += String(val);
    }
  }
  if(config.f1[0] == 1){
    val = 404.0;
    if(config.f1[1] == 0) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
    else if(config.f1[1] == 1) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
    else if(config.f1[1] == 2) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
    else if(config.f1[1] == 3) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
    else if(config.f1[1] == 4) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field1="; 
      url += String(val);
    }
  }
  if(config.f1[0] == 2){
    val = 404.0;
    if(config.f1[1] == 0) val = (sensors.bme280_temp > -55.0 and sensors.bme280_temp < 100.0) ? sensors.bme280_temp : 404.0;
    else if(config.f1[1] == 1) val = (sensors.bme280_hum >= 0.0 and sensors.bme280_hum <= 100.0) ? sensors.bme280_hum : 404.0;
    else if(config.f1[1] == 2) val = (sensors.bme280_pres > 450.0 and sensors.bme280_pres < 1200.0) ? sensors.bme280_pres : 4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field1="; 
      url += String(val);
    }
  }
  if(config.f1[0] == 3){
    val = 404.0;
    if(config.f1[1] == 0) val = (sensors.bmp180_temp > -55.0 and sensors.bmp180_temp < 100.0) ? sensors.bmp180_temp : 404.0;
    else if(config.f1[1] == 2) val = (sensors.bmp180_pres > 450.0 and sensors.bmp180_pres < 1200.0) ? sensors.bme280_pres : 4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field1="; 
      url += String(val);
    }
  }
  if(config.f1[0] == 4){
    val = 404.0;
    if(config.f1[1] == 0) (sensors.sht21_temp > -55.0 and sensors.sht21_temp < 100.0) ? sensors.sht21_temp : 404.0;
    else if(config.f1[1] == 1) val = (sensors.sht21_hum >= 0.0 and sensors.sht21_hum <= 100.0) ? sensors.sht21_hum : 404.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field1="; 
      url += String(val);
    }
  }
  if(config.f1[0] == 5){
    val = 404.0;
    if(config.f1[1] == 0) (sensors.dht22_temp > -55.0 and sensors.dht22_temp < 100.0) ? sensors.dht22_temp : 404.0;
    else if(config.f1[1] == 1) val = (sensors.dht22_hum >= 0.0 and sensors.dht22_hum <= 100.0) ? sensors.dht22_hum : 404.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field1="; 
      url += String(val);
    }
  }
  if(config.f1[0] == 6){
    val = 404.0;
    if(config.f1[1] == 3) val = (sensors.max44009_light >= 0.0 and sensors.max44009_light <= 10000.0) ? sensors.max44009_light : 4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field1="; 
      url += String(val);
    }
  }
  if(config.f1[0] == 7){
    val = 404.0;
    if(config.f1[1] == 3) val = (sensors.bh1750_light >= 0.0 and sensors.bh1750_light <= 10000.0) ? sensors.bh1750_light : 4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field1="; 
      url += String(val);
    }
  }
  if(config.f2[0] == 0){
    val = 404.0;
    if(config.f2[1] == 0) val = datas.temp_web;
    else if(config.f2[1] == 1) val = datas.hum_web;
    else if(config.f2[1] == 2) val = datas.pres_web;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field2="; 
      url += String(val);
    }
  }
  if(config.f2[0] == 1){
    val = 404.0;
    if(config.f2[1] == 0) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
    else if(config.f2[1] == 1) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
    else if(config.f2[1] == 2) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
    else if(config.f2[1] == 3) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
    else if(config.f2[1] == 4) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field2="; 
      url += String(val);
    }
  }
  if(config.f2[0] == 2){
    val = 404.0;
    if(config.f2[1] == 0) val = sensors.bme280_temp;
    else if(config.f2[1] == 1) val = sensors.bme280_hum;
    else if(config.f2[1] == 2) val = sensors.bme280_pres;
    if(val != 202 and val != 404.0 and val != 4040.0)
    {
      url += "&field2="; 
      url += String(val);
    }
  }
  if(config.f2[0] == 3){
    val = 404.0;
    if(config.f2[1] == 0) val = sensors.bmp180_temp;
    else if(config.f2[1] == 2) val = sensors.bmp180_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field2="; 
      url += String(val);
    }
  }
  if(config.f2[0] == 4){
    val = 404.0;
    if(config.f2[1] == 0) val = sensors.sht21_temp;
    else if(config.f2[1] == 1) val = sensors.sht21_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field2="; 
      url += String(val);
    }
  }
  if(config.f2[0] == 5){
    val = 404.0;
    if(config.f2[1] == 0) val = sensors.dht22_temp;
    else if(config.f2[1] == 1) val = sensors.dht22_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field2="; 
      url += String(val);
    }
  }
  if(config.f2[0] == 6){
    val = 404.0;
    if(config.f2[1] == 3) val = sensors.max44009_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field2="; 
      url += String(val);
    }
  }
  if(config.f2[0] == 7){
    val = 404.0;
    if(config.f2[1] == 3) val = sensors.bh1750_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field2="; 
      url += String(val);
    }
  }
  if(config.f3[0] == 0){
    val = 404.0;
    if(config.f3[1] == 0) val = datas.temp_web;
    else if(config.f3[1] == 1) val = datas.hum_web;
    else if(config.f3[1] == 2) val = datas.pres_web;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field3="; 
      url += String(val);
    }
  }
  if(config.f3[0] == 1){
    val = 404.0;
    if(config.f3[1] == 0) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
    else if(config.f3[1] == 1) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
    else if(config.f3[1] == 2) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
    else if(config.f3[1] == 3) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
    else if(config.f3[1] == 4) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field3="; 
      url += String(val);
    }
  }
  if(config.f3[0] == 2){
    val = 404.0;
    if(config.f3[1] == 0) val = sensors.bme280_temp;
    else if(config.f3[1] == 1) val = sensors.bme280_hum;
    else if(config.f3[1] == 2) val = sensors.bme280_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field3="; 
      url += String(val);
    }
  }
  if(config.f3[0] == 3){
    val = 404.0;
    if(config.f3[1] == 0) val = sensors.bmp180_temp;
    else if(config.f3[1] == 2) val = sensors.bmp180_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field3="; 
      url += String(val);
    }
  }
  if(config.f3[0] == 4){
    val = 404.0;
    if(config.f3[1] == 0) val = sensors.sht21_temp;
    else if(config.f3[1] == 1) val = sensors.sht21_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field3="; 
      url += String(val);
    }
  }
  if(config.f3[0] == 5){
    val = 404.0;
    if(config.f3[1] == 0) val = sensors.dht22_temp;
    else if(config.f3[1] == 1) val = sensors.dht22_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field3="; 
      url += String(val);
    }
  }
  if(config.f3[0] == 6){
    val = 404.0;
    if(config.f3[1] == 3) val = sensors.max44009_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field3="; 
      url += String(val);
    }
  }
  if(config.f3[0] == 7){
    val = 404.0;
    if(config.f3[1] == 3) val = sensors.bh1750_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field3="; 
      url += String(val);
    }
  }
  if(config.f4[0] == 0){
    val = 404.0;
    if(config.f4[1] == 0) val = datas.temp_web;
    else if(config.f4[1] == 1) val = datas.hum_web;
    else if(config.f4[1] == 2) val = datas.pres_web;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field4="; 
      url += String(val);
    }
  }
  if(config.f4[0] == 1){
    val = 404.0;
    if(config.f4[1] == 0) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
    else if(config.f4[1] == 1) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
    else if(config.f4[1] == 2) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
    else if(config.f4[1] == 3) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
    else if(config.f4[1] == 4) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field4="; 
      url += String(val);
    }
  }
  if(config.f4[0] == 2){
    val = 404.0;
    if(config.f4[1] == 0) val = sensors.bme280_temp;
    else if(config.f4[1] == 1) val = sensors.bme280_hum;
    else if(config.f4[1] == 2) val = sensors.bme280_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field4="; 
      url += String(val);
    }
  }
  if(config.f4[0] == 3){
    val = 404.0;
    if(config.f4[1] == 0) val = sensors.bmp180_temp;
    else if(config.f4[1] == 2) val = sensors.bmp180_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field4="; 
      url += String(val);
    }
  }
  if(config.f4[0] == 4){
    val = 404.0;
    if(config.f4[1] == 0) val = sensors.sht21_temp;
    else if(config.f4[1] == 1) val = sensors.sht21_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field4="; 
      url += String(val);
    }
  }
  if(config.f4[0] == 5){
    val = 404.0;
    if(config.f4[1] == 0) val = sensors.dht22_temp;
    else if(config.f4[1] == 1) val = sensors.dht22_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field4="; 
      url += String(val);
    }
  }
  if(config.f4[0] == 6){
    val = 404.0;
    if(config.f4[1] == 3) val = sensors.max44009_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field4="; 
      url += String(val);
    }
  }
  if(config.f4[0] == 7){
    val = 404.0;
    if(config.f4[1] == 3) val = sensors.bh1750_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field4="; 
      url += String(val);
    }
  }
  if(config.f5[0] == 0){
    val = 404.0;
    if(config.f5[1] == 0) val = datas.temp_web;
    else if(config.f5[1] == 1) val = datas.hum_web;
    else if(config.f5[1] == 2) val = datas.pres_web;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field5="; 
      url += String(val);
    }
  }
  if(config.f5[0] == 1){
    val = 404.0;
    if(config.f5[1] == 0) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
    else if(config.f5[1] == 1) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
    else if(config.f5[1] == 2) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
    else if(config.f5[1] == 3) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
    else if(config.f5[1] == 4) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field5="; 
      url += String(val);
    }
  }
  if(config.f5[0] == 2){
    val = 404.0;
    if(config.f5[1] == 0) val = sensors.bme280_temp;
    else if(config.f5[1] == 1) val = sensors.bme280_hum;
    else if(config.f5[1] == 2) val = sensors.bme280_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field5="; 
      url += String(val);
    }
  }
  if(config.f5[0] == 3){
    val = 404.0;
    if(config.f5[1] == 0) val = sensors.bmp180_temp;
    else if(config.f5[1] == 2) val = sensors.bmp180_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field5="; 
      url += String(val);
    }
  }
  if(config.f5[0] == 4){
    val = 404.0;
    if(config.f5[1] == 0) val = sensors.sht21_temp;
    else if(config.f5[1] == 1) val = sensors.sht21_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field5="; 
      url += String(val);
    }
  }
  if(config.f5[0] == 5){
    val = 404.0;
    if(config.f5[1] == 0) val = sensors.dht22_temp;
    else if(config.f5[1] == 1) val = sensors.dht22_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field5="; 
      url += String(val);
    }
  }
  if(config.f5[0] == 6){
    val = 404.0;
    if(config.f5[1] == 3) val = sensors.max44009_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field5="; 
      url += String(val);
    }
  }
  if(config.f5[0] == 7){
    val = 404.0;
    if(config.f5[1] == 3) val = sensors.bh1750_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field5="; 
      url += String(val);
    }
  }
  if(config.f6[0] == 0){
    val = 404.0;
    if(config.f6[1] == 0) val = datas.temp_web;
    else if(config.f6[1] == 1) val = datas.hum_web;
    else if(config.f6[1] == 2) val = datas.pres_web;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field6="; 
      url += String(val);
    }
  }
  if(config.f6[0] == 1){
    val = 404.0;
    if(config.f6[1] == 0) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
    else if(config.f6[1] == 1) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
    else if(config.f6[1] == 2) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
    else if(config.f6[1] == 3) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
    else if(config.f6[1] == 4) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field6="; 
      url += String(val);
    }
  }
  if(config.f6[0] == 2){
    val = 404.0;
    if(config.f6[1] == 0) val = sensors.bme280_temp;
    else if(config.f6[1] == 1) val = sensors.bme280_hum;
    else if(config.f6[1] == 2) val = sensors.bme280_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field6="; 
      url += String(val);
    }
  }
  if(config.f6[0] == 3){
    val = 404.0;
    if(config.f6[1] == 0) val = sensors.bmp180_temp;
    else if(config.f6[1] == 2) val = sensors.bmp180_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field6="; 
      url += String(val);
    }
  }
  if(config.f6[0] == 4){
    val = 404.0;
    if(config.f6[1] == 0) val = sensors.sht21_temp;
    else if(config.f6[1] == 1) val = sensors.sht21_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field6="; 
      url += String(val);
    }
  }
  if(config.f6[0] == 5){
    val = 404.0;
    if(config.f6[1] == 0) val = sensors.dht22_temp;
    else if(config.f6[1] == 1) val = sensors.dht22_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field6="; 
      url += String(val);
    }
  }
  if(config.f6[0] == 6){
    val = 404.0;
    if(config.f6[1] == 3) val = sensors.max44009_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field6="; 
      url += String(val);
    }
  }
  if(config.f6[0] == 7){
    val = 404.0;
    if(config.f6[1] == 3) val = sensors.bh1750_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field6="; 
      url += String(val);
    }
  }
  if(config.f7[0] == 0){
    val = 404.0;
    if(config.f7[1] == 0) val = datas.temp_web;
    else if(config.f7[1] == 1) val = datas.hum_web;
    else if(config.f7[1] == 2) val = datas.pres_web;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field7="; 
      url += String(val);
    }
  }
  if(config.f7[0] == 1){
    val = 404.0;
    if(config.f7[1] == 0) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
    else if(config.f7[1] == 1) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
    else if(config.f7[1] == 2) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
    else if(config.f7[1] == 3) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
    else if(config.f7[1] == 4) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field7="; 
      url += String(val);
    }
  }
  if(config.f7[0] == 2){
    val = 404.0;
    if(config.f7[1] == 0) val = sensors.bme280_temp;
    else if(config.f7[1] == 1) val = sensors.bme280_hum;
    else if(config.f7[1] == 2) val = sensors.bme280_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field7="; 
      url += String(val);
    }
  }
  if(config.f7[0] == 3){
    val = 404.0;
    if(config.f7[1] == 0) val = sensors.bmp180_temp;
    else if(config.f7[1] == 2) val = sensors.bmp180_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field7="; 
      url += String(val);
    }
  }
  if(config.f7[0] == 4){
    val = 404.0;
    if(config.f7[1] == 0) val = sensors.sht21_temp;
    else if(config.f7[1] == 1) val = sensors.sht21_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field7="; 
      url += String(val);
    }
  }
  if(config.f7[0] == 5){
    val = 404.0;
    if(config.f7[1] == 0) val = sensors.dht22_temp;
    else if(config.f7[1] == 1) val = sensors.dht22_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field7="; 
      url += String(val);
    }
  }
  if(config.f7[0] == 6){
    val = 404.0;
    if(config.f7[1] == 3) val = sensors.max44009_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field7="; 
      url += String(val);
    }
  }
  if(config.f7[0] == 7){
    val = 404.0;
    if(config.f7[1] == 3) val = sensors.bh1750_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field7="; 
      url += String(val);
    }
  }
  if(config.f8[0] == 0){
    val = 404.0;
    if(config.f8[1] == 0) val = datas.temp_web;
    else if(config.f8[1] == 1) val = datas.hum_web;
    else if(config.f8[1] == 2) val = datas.pres_web;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field8="; 
      url += String(val);
    }
  }
  if(config.f8[0] == 1){
    val = 404.0;
    if(config.f8[1] == 0) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
    else if(config.f8[1] == 1) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
    else if(config.f8[1] == 2) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
    else if(config.f8[1] == 3) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
    else if(config.f8[1] == 4) val = ((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field8="; 
      url += String(val);
    }
  }
  if(config.f8[0] == 2){
    val = 404.0;
    if(config.f8[1] == 0) val = sensors.bme280_temp;
    else if(config.f8[1] == 1) val = sensors.bme280_hum;
    else if(config.f8[1] == 2) val = sensors.bme280_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field8="; 
      url += String(val);
    }
  }
  if(config.f8[0] == 3){
    val = 404.0;
    if(config.f8[1] == 0) val = sensors.bmp180_temp;
    else if(config.f8[1] == 2) val = sensors.bmp180_pres;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field8="; 
      url += String(val);
    }
  }
  if(config.f8[0] == 4){
    val = 404.0;
    if(config.f8[1] == 0) val = sensors.sht21_temp;
    else if(config.f8[1] == 1) val = sensors.sht21_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field8="; 
      url += String(val);
    }
  }
  if(config.f8[0] == 5){
    val = 404.0;
    if(config.f8[1] == 0) val = sensors.dht22_temp;
    else if(config.f8[1] == 1) val = sensors.dht22_hum;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field8="; 
      url += String(val);
    }
  }
  if(config.f8[0] == 6){
    val = 404.0;
    if(config.f8[1] == 3) val = sensors.max44009_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field8="; 
      url += String(val);
    }
  }
  if(config.f8[0] == 7){
    val = 404.0;
    if(config.f8[1] == 3) val = sensors.bh1750_light;
    if(val != 202 and val != 404.0 and val != 4040.0){
      url += "&field8="; 
      url += String(val);
    }
  }
  String httpData = "";
  HTTPClient th_client;
  th_client.begin(url);
  int httpCode = th_client.GET();
  if(httpCode > 0){
    if(httpCode == HTTP_CODE_OK){
      httpData = th_client.getString();
      Serial.print("Channel update successful: ");
      Serial.println(httpData);
    }
    else Serial.println("Problem updating channel. HTTP error code " + String(httpCode));
  }
  th_client.end();
  httpData = "";
}

void MQTT_connect(Adafruit_MQTT_Client &mqtt){
  int8_t ret;
  if(mqtt.connected()) return;
  Serial.print("Connecting to MQTT... ");
  int8_t retries = 5;
  while((ret = mqtt.connect()) != 0){
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    vTaskDelay(5000);
    retries--;
    if(retries <= 0) return;
  }
  Serial.println("MQTT Connected!");
}

void TaskWeather(void *pvParameters){
  (void) pvParameters;
  time_t weathr = millis();
  bool first_upd = true;
  float val = 404.0;
  Serial.print("config.mqttServer "); Serial.println(config.mqttServer);
  Serial.print("config.mqttPort "); Serial.println(config.mqttPort);
  Serial.print("config.mqttLogin "); Serial.println(config.mqttLogin);
  Serial.print("config.mqttPass "); Serial.println(config.mqttPass);
  Adafruit_MQTT_Client mqtt(&client, config.mqttServer, config.mqttPort, config.mqttLogin, config.mqttPass);
  Serial.print("config.mqttF1 "); Serial.println(config.mqttF1);
  Adafruit_MQTT_Publish topic1 = Adafruit_MQTT_Publish(&mqtt, config.mqttF1);
  Serial.print("config.mqttF2 "); Serial.println(config.mqttF2);
  Adafruit_MQTT_Publish topic2 = Adafruit_MQTT_Publish(&mqtt, config.mqttF2);
  Serial.print("config.mqttF3 "); Serial.println(config.mqttF3);
  Adafruit_MQTT_Publish topic3 = Adafruit_MQTT_Publish(&mqtt, config.mqttF3);
  Serial.print("config.mqttF4 "); Serial.println(config.mqttF4);
  Adafruit_MQTT_Publish topic4 = Adafruit_MQTT_Publish(&mqtt, config.mqttF4);
  Serial.print("config.mqttF5 "); Serial.println(config.mqttF5);
  Adafruit_MQTT_Publish topic5 = Adafruit_MQTT_Publish(&mqtt, config.mqttF5);
  Serial.print("config.mqttF6 "); Serial.println(config.mqttF6);
  Adafruit_MQTT_Publish topic6 = Adafruit_MQTT_Publish(&mqtt, config.mqttF6);
  Serial.print("config.mqttF7 "); Serial.println(config.mqttF7);
  Adafruit_MQTT_Publish topic7 = Adafruit_MQTT_Publish(&mqtt, config.mqttF7);
  Serial.print("config.mqttF8 "); Serial.println(config.mqttF8);
  Adafruit_MQTT_Publish topic8 = Adafruit_MQTT_Publish(&mqtt, config.mqttF8);
  Adafruit_MQTT_Subscribe top1 = Adafruit_MQTT_Subscribe(&mqtt, config.mqttT1);
  Adafruit_MQTT_Subscribe top2 = Adafruit_MQTT_Subscribe(&mqtt, config.mqttT2);
  Adafruit_MQTT_Subscribe top3 = Adafruit_MQTT_Subscribe(&mqtt, config.mqttT3);
  Adafruit_MQTT_Subscribe top4 = Adafruit_MQTT_Subscribe(&mqtt, config.mqttT4);
  Adafruit_MQTT_Subscribe top5 = Adafruit_MQTT_Subscribe(&mqtt, config.mqttT5);
  Adafruit_MQTT_Subscribe top6 = Adafruit_MQTT_Subscribe(&mqtt, config.mqttT6);
  Adafruit_MQTT_Subscribe top7 = Adafruit_MQTT_Subscribe(&mqtt, config.mqttT7);
  Adafruit_MQTT_Subscribe top8 = Adafruit_MQTT_Subscribe(&mqtt, config.mqttT8);
  while(!datas.net_connected) vTaskDelay(10); 
  if(config.tq1) mqtt.subscribe(&top1);
  if(config.tq2) mqtt.subscribe(&top2);
  if(config.tq3) mqtt.subscribe(&top3);
  if(config.tq4) mqtt.subscribe(&top4);
  if(config.tq5) mqtt.subscribe(&top5);
  if(config.tq6) mqtt.subscribe(&top6);
  if(config.tq7) mqtt.subscribe(&top7);
  if(config.tq8) mqtt.subscribe(&top8);
  
  while(1){
    if(datas.net_connected and (millis() - weathr > 1200000 or first_upd)){
      Serial.println("WEATHER UPDATING");
      datas.weather_updating = true;
      getWeatherNow();
      getWeatherDaily();
      datas.weather_updating = false;
      datas.old_ant = 0;
      first_upd = false;
      Serial.println("WEATHER UPDATED");
      weathr = millis();
    }
    else if(config.mqttsend or config.mqttrcv){
      vTaskDelay(5000);
      MQTT_connect(mqtt);
      Adafruit_MQTT_Subscribe *subscription;
      while((subscription = mqtt.readSubscription(3000))){
        float rtop1 = 4040.0;
        float rtop2 = 4040.0;
        float rtop3 = 4040.0;
        float rtop4 = 4040.0;
        float rtop5 = 4040.0;
        float rtop6 = 4040.0;
        float rtop7 = 4040.0;
        float rtop8 = 4040.0;
        Serial.println("mqtt received");
        if(config.tq1 and subscription == &top1) rtop1 = atof((char *)top1.lastread);
        if(config.tq2 and subscription == &top2) rtop2 = atof((char *)top2.lastread);
        if(config.tq3 and subscription == &top3) rtop3 = atof((char *)top3.lastread);
        if(config.tq4 and subscription == &top4) rtop4 = atof((char *)top4.lastread);
        if(config.tq5 and subscription == &top5) rtop5 = atof((char *)top5.lastread);
        if(config.tq6 and subscription == &top6) rtop6 = atof((char *)top6.lastread);
        if(config.tq7 and subscription == &top7) rtop7 = atof((char *)top7.lastread);
        if(config.tq8 and subscription == &top8) rtop8 = atof((char *)top8.lastread);
        if(config.mqttrcv){
          switch(config.mto){
            case 1: if(rtop1 != 4040.0) datas.mqtt_temp_out = rtop1; break;
            case 2: if(rtop2 != 4040.0) datas.mqtt_temp_out = rtop2; break;
            case 3: if(rtop3 != 4040.0) datas.mqtt_temp_out = rtop3; break;
            case 4: if(rtop4 != 4040.0) datas.mqtt_temp_out = rtop4; break;
            case 5: if(rtop5 != 4040.0) datas.mqtt_temp_out = rtop5; break;
            case 6: if(rtop6 != 4040.0) datas.mqtt_temp_out = rtop6; break;
            case 7: if(rtop7 != 4040.0) datas.mqtt_temp_out = rtop7; break;
            case 8: if(rtop8 != 4040.0) datas.mqtt_temp_out = rtop8; break;
            default: datas.mqtt_temp_out = 404.0; break;        
          }
          switch(config.mti){
            case 1: if(rtop1 != 4040.0) datas.mqtt_temp_in = rtop1; break;
            case 2: if(rtop2 != 4040.0) datas.mqtt_temp_in = rtop2; break;
            case 3: if(rtop3 != 4040.0) datas.mqtt_temp_in = rtop3; break;
            case 4: if(rtop4 != 4040.0) datas.mqtt_temp_in = rtop4; break;
            case 5: if(rtop5 != 4040.0) datas.mqtt_temp_in = rtop5; break;
            case 6: if(rtop6 != 4040.0) datas.mqtt_temp_in = rtop6; break;
            case 7: if(rtop7 != 4040.0) datas.mqtt_temp_in = rtop7; break;
            case 8: if(rtop8 != 4040.0) datas.mqtt_temp_in = rtop8; break;
            default: datas.mqtt_temp_in = 404.0; break;        
          }
          switch(config.mho){
            case 1: if(rtop1 != 4040.0) datas.mqtt_hum_out = rtop1; break;
            case 2: if(rtop2 != 4040.0) datas.mqtt_hum_out = rtop2; break;
            case 3: if(rtop3 != 4040.0) datas.mqtt_hum_out = rtop3; break;
            case 4: if(rtop4 != 4040.0) datas.mqtt_hum_out = rtop4; break;
            case 5: if(rtop5 != 4040.0) datas.mqtt_hum_out = rtop5; break;
            case 6: if(rtop6 != 4040.0) datas.mqtt_hum_out = rtop6; break;
            case 7: if(rtop7 != 4040.0) datas.mqtt_hum_out = rtop7; break;
            case 8: if(rtop8 != 4040.0) datas.mqtt_hum_out = rtop8; break;
            default: datas.mqtt_hum_out = 404.0; break;        
          }
          switch(config.mhi){
            case 1: if(rtop1 != 4040.0) datas.mqtt_hum_in = rtop1; break;
            case 2: if(rtop2 != 4040.0) datas.mqtt_hum_in = rtop2; break;
            case 3: if(rtop3 != 4040.0) datas.mqtt_hum_in = rtop3; break;
            case 4: if(rtop4 != 4040.0) datas.mqtt_hum_in = rtop4; break;
            case 5: if(rtop5 != 4040.0) datas.mqtt_hum_in = rtop5; break;
            case 6: if(rtop6 != 4040.0) datas.mqtt_hum_in = rtop6; break;
            case 7: if(rtop7 != 4040.0) datas.mqtt_hum_in = rtop7; break;
            case 8: if(rtop8 != 4040.0) datas.mqtt_hum_in = rtop8; break;
            default: datas.mqtt_hum_in = 404.0; break;        
          }
          switch(config.mpo){
            case 1: if(rtop1 != 4040.0) datas.mqtt_pres_out = rtop1; break;
            case 2: if(rtop2 != 4040.0) datas.mqtt_pres_out = rtop2; break;
            case 3: if(rtop3 != 4040.0) datas.mqtt_pres_out = rtop3; break;
            case 4: if(rtop4 != 4040.0) datas.mqtt_pres_out = rtop4; break;
            case 5: if(rtop5 != 4040.0) datas.mqtt_pres_out = rtop5; break;
            case 6: if(rtop6 != 4040.0) datas.mqtt_pres_out = rtop6; break;
            case 7: if(rtop7 != 4040.0) datas.mqtt_pres_out = rtop7; break;
            case 8: if(rtop8 != 4040.0) datas.mqtt_pres_out = rtop8; break;
            default: datas.mqtt_pres_out = 404.0; break;        
          }
          switch(config.mli){
            case 1: if(rtop1 != 4040.0) datas.mqtt_light_in = rtop1; break;
            case 2: if(rtop2 != 4040.0) datas.mqtt_light_in = rtop2; break;
            case 3: if(rtop3 != 4040.0) datas.mqtt_light_in = rtop3; break;
            case 4: if(rtop4 != 4040.0) datas.mqtt_light_in = rtop4; break;
            case 5: if(rtop5 != 4040.0) datas.mqtt_light_in = rtop5; break;
            case 6: if(rtop6 != 4040.0) datas.mqtt_light_in = rtop6; break;
            case 7: if(rtop7 != 4040.0) datas.mqtt_light_in = rtop7; break;
            case 8: if(rtop8 != 4040.0) datas.mqtt_light_in = rtop8; break;
            default: datas.mqtt_light_in = 404.0; break;        
          }
          Serial.print("mqtt_temp_out="); Serial.println(datas.mqtt_temp_out);
          Serial.print("mqtt_temp_in="); Serial.println(datas.mqtt_temp_in);
          Serial.print("mqtt_hum_out="); Serial.println(datas.mqtt_hum_out);
          Serial.print("mqtt_hum_in="); Serial.println(datas.mqtt_hum_in);
          Serial.print("mqtt_pres_out="); Serial.println(datas.mqtt_pres_out);
          Serial.print("mqtt_light_in="); Serial.println(datas.mqtt_light_in);
        }
      }
      if(datas.mqtt_sending){
        char str[32];
        Serial.println("mqtt send");
        MQTT_connect(mqtt);
        if(config.fq1){
          Serial.println("config.fq1");
          if(config.fd1[0] == 0){
            val = 404.0;
            if(config.fd1[1] == 0) val = datas.temp_web;
            else if(config.fd1[1] == 1) val = datas.hum_web;
            else if(config.fd1[1] == 2) val = datas.pres_web;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic1.publish(str);
              Serial.println(str);
            }
          }
          if(config.fd1[0] == 1){
            val = 404.0;
            if(config.fd1[1] == 0) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
            else if(config.fd1[1] == 1) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
            else if(config.fd1[1] == 2) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
            else if(config.fd1[1] == 3) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
            else if(config.fd1[1] == 4) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic1.publish(str);
            }
          }
          if(config.fd1[0] == 2){
            val = 404.0;
            if(config.fd1[1] == 0) val = sensors.bme280_temp;
            else if(config.fd1[1] == 1) val = sensors.bme280_hum;
            else if(config.fd1[1] == 2) val = sensors.bme280_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic1.publish(str);
            }
          }
          if(config.fd1[0] == 3){
            val = 404.0;
            if(config.fd1[1] == 0) val = sensors.bmp180_temp;
            else if(config.fd1[1] == 2) val = sensors.bmp180_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic1.publish(str);
            }
          }
          if(config.fd1[0] == 4){
            val = 404.0;
            if(config.fd1[1] == 0) val = sensors.sht21_temp;
            else if(config.fd1[1] == 1) val = sensors.sht21_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic1.publish(str);
            }
          }
          if(config.fd1[0] == 5){
            val = 404.0;
            if(config.fd1[1] == 0) val = sensors.dht22_temp;
            else if(config.fd1[1] == 1) val = sensors.dht22_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic1.publish(str);
            }
          }
          if(config.fd1[0] == 6){
            val = 404.0;
            if(config.fd1[1] == 3) val = sensors.max44009_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic1.publish(str);
            }
          }
          if(config.fd1[0] == 7){
            val = 404.0;
            if(config.fd1[1] == 3) val = sensors.bh1750_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic1.publish(str);
            }
          }
        }
        if(config.fq2){
          if(config.fd2[0] == 0){
            val = 404.0;
            if(config.fd2[1] == 0) val = datas.temp_web;
            else if(config.fd2[1] == 1) val = datas.hum_web;
            else if(config.fd2[1] == 2) val = datas.pres_web;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic2.publish(str);
            }
          }
          if(config.fd2[0] == 1){
            val = 404.0;
            if(config.fd2[1] == 0) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
            else if(config.fd2[1] == 1) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
            else if(config.fd2[1] == 2) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
            else if(config.fd2[1] == 3) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
            else if(config.fd2[1] == 4) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic2.publish(str);
            }
          }
          if(config.fd2[0] == 2){
            val = 404.0;
            if(config.fd2[1] == 0) val = sensors.bme280_temp;
            else if(config.fd2[1] == 1) val = sensors.bme280_hum;
            else if(config.fd2[1] == 2) val = sensors.bme280_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic2.publish(str);
            }
          }
          if(config.fd2[0] == 3){
            val = 404.0;
            if(config.fd2[1] == 0) val = sensors.bmp180_temp;
            else if(config.fd2[1] == 2) val = sensors.bmp180_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic2.publish(str);
            }
          }
          if(config.fd2[0] == 4){
            val = 404.0;
            if(config.fd2[1] == 0) val = sensors.sht21_temp;
            else if(config.fd2[1] == 1) val = sensors.sht21_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic2.publish(str);
            }
          }
          if(config.fd2[0] == 5){
            val = 404.0;
            if(config.fd2[1] == 0) val = sensors.dht22_temp;
            else if(config.fd2[1] == 1) val = sensors.dht22_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic2.publish(str);
            }
          }
          if(config.fd2[0] == 6){
            val = 404.0;
            if(config.fd2[1] == 3) val = sensors.max44009_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic2.publish(str);
            }
          }
          if(config.fd2[0] == 7){
            val = 404.0;
            if(config.fd2[1] == 3) val = sensors.bh1750_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic2.publish(str);
            }
          }
        }
        if(config.fq3){ 
          if(config.fd3[0] == 0){
            val = 404.0;
            if(config.fd3[1] == 0) val = datas.temp_web;
            else if(config.fd3[1] == 1) val = datas.hum_web;
            else if(config.fd3[1] == 2) val = datas.pres_web;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic3.publish(str);
            }
          }
          if(config.fd3[0] == 1){
            val = 404.0;
            if(config.fd3[1] == 0) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
            else if(config.fd3[1] == 1) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
            else if(config.fd3[1] == 2) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
            else if(config.fd3[1] == 3) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
            else if(config.fd3[1] == 4) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic3.publish(str);
            }
          }
          if(config.fd3[0] == 2){
            val = 404.0;
            if(config.fd3[1] == 0) val = sensors.bme280_temp;
            else if(config.fd3[1] == 1) val = sensors.bme280_hum;
            else if(config.fd3[1] == 2) val = sensors.bme280_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic3.publish(str);
            }
          }
          if(config.fd3[0] == 3){
            val = 404.0;
            if(config.fd3[1] == 0) val = sensors.bmp180_temp;
            else if(config.fd3[1] == 2) val = sensors.bmp180_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic3.publish(str);
            }
          }
          if(config.fd3[0] == 4){
            val = 404.0;
            if(config.fd3[1] == 0) val = sensors.sht21_temp;
            else if(config.fd3[1] == 1) val = sensors.sht21_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic3.publish(str);
            }
          }
          if(config.fd3[0] == 5){
            val = 404.0;
            if(config.fd3[1] == 0) val = sensors.dht22_temp;
            else if(config.fd3[1] == 1) val = sensors.dht22_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic3.publish(str);
            }
          }
          if(config.fd3[0] == 6){
            val = 404.0;
            if(config.fd3[1] == 3) val = sensors.max44009_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic3.publish(str);
            }
          }
          if(config.fd3[0] == 7){
            val = 404.0;
            if(config.fd3[1] == 3) val = sensors.bh1750_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic3.publish(str);
            }
          }
        }
        if(config.fq4){
          if(config.fd4[0] == 0){
            val = 404.0;
            if(config.fd4[1] == 0) val = datas.temp_web;
            else if(config.fd4[1] == 1) val = datas.hum_web;
            else if(config.fd4[1] == 2) val = datas.pres_web;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic4.publish(str);
            }
          }
          if(config.fd4[0] == 1){
            val = 404.0;
            if(config.fd4[1] == 0) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
            else if(config.fd4[1] == 1) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
            else if(config.fd4[1] == 2) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
            else if(config.fd4[1] == 3) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
            else if(config.fd4[1] == 4) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic4.publish(str);
            }
          }
          if(config.fd4[0] == 2){
            val = 404.0;
            if(config.fd4[1] == 0) val = sensors.bme280_temp;
            else if(config.fd4[1] == 1) val = sensors.bme280_hum;
            else if(config.fd4[1] == 2) val = sensors.bme280_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic4.publish(str);
            }
          }
          if(config.fd4[0] == 3){
            val = 404.0;
            if(config.fd4[1] == 0) val = sensors.bmp180_temp;
            else if(config.fd4[1] == 2) val = sensors.bmp180_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic4.publish(str);
            }
          }
          if(config.fd4[0] == 4){
            val = 404.0;
            if(config.fd4[1] == 0) val = sensors.sht21_temp;
            else if(config.fd4[1] == 1) val = sensors.sht21_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic4.publish(str);
            }
          }
          if(config.fd4[0] == 5){
            val = 404.0;
            if(config.fd4[1] == 0) val = sensors.dht22_temp;
            else if(config.fd4[1] == 1) val = sensors.dht22_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic4.publish(str);
            }
          }
          if(config.fd4[0] == 6){
            val = 404.0;
            if(config.fd4[1] == 3) val = sensors.max44009_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic4.publish(str);
            }
          }
          if(config.fd4[0] == 7){
            val = 404.0;
            if(config.fd4[1] == 3) val = sensors.bh1750_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic4.publish(str);
            }
          }
        }
        if(config.fq5){
          if(config.fd5[0] == 0){
            val = 404.0;
            if(config.fd5[1] == 0) val = datas.temp_web;
            else if(config.fd5[1] == 1) val = datas.hum_web;
            else if(config.fd5[1] == 2) val = datas.pres_web;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic5.publish(str);
            }
          }
          if(config.fd5[0] == 1){
            val = 404.0;
            if(config.fd5[1] == 0) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
            else if(config.fd5[1] == 1) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
            else if(config.fd5[1] == 2) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
            else if(config.fd5[1] == 3) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
            else if(config.fd5[1] == 4) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic5.publish(str);
            }
          }
          if(config.fd5[0] == 2){
            val = 404.0;
            if(config.fd5[1] == 0) val = sensors.bme280_temp;
            else if(config.fd5[1] == 1) val = sensors.bme280_hum;
            else if(config.fd5[1] == 2) val = sensors.bme280_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic5.publish(str);
            }
          }
          if(config.fd5[0] == 3){
            val = 404.0;
            if(config.fd5[1] == 0) val = sensors.bmp180_temp;
            else if(config.fd5[1] == 2) val = sensors.bmp180_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic5.publish(str);
            }
          }
          if(config.fd5[0] == 4){
            val = 404.0;
            if(config.fd5[1] == 0) val = sensors.sht21_temp;
            else if(config.fd5[1] == 1) val = sensors.sht21_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic5.publish(str);
            }
          }
          if(config.fd5[0] == 5){
            val = 404.0;
            if(config.fd5[1] == 0) val = sensors.dht22_temp;
            else if(config.fd5[1] == 1) val = sensors.dht22_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic5.publish(str);
            }
          }
          if(config.fd5[0] == 6){
            val = 404.0;
            if(config.fd5[1] == 3) val = sensors.max44009_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic5.publish(str);
            }
          }
          if(config.fd5[0] == 7){
            val = 404.0;
            if(config.fd5[1] == 3) val = sensors.bh1750_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic5.publish(str);
            }
          }
        }
        if(config.fq6){
          if(config.fd6[0] == 0){
            val = 404.0;
            if(config.fd6[1] == 0) val = datas.temp_web;
            else if(config.fd6[1] == 1) val = datas.hum_web;
            else if(config.fd6[1] == 2) val = datas.pres_web;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic6.publish(str);
            }
          }
          if(config.fd6[0] == 1){
            val = 404.0;
            if(config.fd6[1] == 0) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
            else if(config.fd6[1] == 1) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
            else if(config.fd6[1] == 2) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
            else if(config.fd6[1] == 3) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
            else if(config.fd6[1] == 4) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic6.publish(str);
            }
          }
          if(config.fd6[0] == 2){
            val = 404.0;
            if(config.fd6[1] == 0) val = sensors.bme280_temp;
            else if(config.fd6[1] == 1) val = sensors.bme280_hum;
            else if(config.fd6[1] == 2) val = sensors.bme280_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic6.publish(str);
            }
          }
          if(config.fd6[0] == 3){
            val = 404.0;
            if(config.fd6[1] == 0) val = sensors.bmp180_temp;
            else if(config.fd6[1] == 2) val = sensors.bmp180_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic6.publish(str);
            }
          }
          if(config.fd6[0] == 4){
            val = 404.0;
            if(config.fd6[1] == 0) val = sensors.sht21_temp;
            else if(config.fd6[1] == 1) val = sensors.sht21_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic6.publish(str);
            }
          }
          if(config.fd6[0] == 5){
            val = 404.0;
            if(config.fd6[1] == 0) val = sensors.dht22_temp;
            else if(config.fd6[1] == 1) val = sensors.dht22_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic6.publish(str);
            }
          }
          if(config.fd6[0] == 6){
            val = 404.0;
            if(config.fd6[1] == 3) val = sensors.max44009_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic6.publish(str);
            }
          }
          if(config.fd6[0] == 7){
            val = 404.0;
            if(config.fd6[1] == 3) val = sensors.bh1750_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic6.publish(str);
            }
          }
        }
        if(config.fq7){
          if(config.fd7[0] == 0){
            val = 404.0;
            if(config.fd7[1] == 0) val = datas.temp_web;
            else if(config.fd7[1] == 1) val = datas.hum_web;
            else if(config.fd7[1] == 2) val = datas.pres_web;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic7.publish(str);
            }
          }
          if(config.fd7[0] == 1){
            val = 404.0;
            if(config.fd7[1] == 0) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
            else if(config.fd7[1] == 1) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
            else if(config.fd7[1] == 2) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
            else if(config.fd7[1] == 3) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
            else if(config.fd7[1] == 4) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic7.publish(str);
            }
          }
          if(config.fd7[0] == 2){
            val = 404.0;
            if(config.fd7[1] == 0) val = sensors.bme280_temp;
            else if(config.fd7[1] == 1) val = sensors.bme280_hum;
            else if(config.fd7[1] == 2) val = sensors.bme280_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic7.publish(str);
            }
          }
          if(config.fd7[0] == 3){
            val = 404.0;
            if(config.fd7[1] == 0) val = sensors.bmp180_temp;
            else if(config.fd7[1] == 2) val = sensors.bmp180_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic7.publish(str);
            }
          }
          if(config.fd7[0] == 4){
            val = 404.0;
            if(config.fd7[1] == 0) val = sensors.sht21_temp;
            else if(config.fd7[1] == 1) val = sensors.sht21_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic7.publish(str);
            }
          }
          if(config.fd7[0] == 5){
            val = 404.0;
            if(config.fd7[1] == 0) val = sensors.dht22_temp;
            else if(config.fd7[1] == 1) val = sensors.dht22_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic7.publish(str);
            }
          }
          if(config.fd7[0] == 6){
            val = 404.0;
            if(config.fd7[1] == 3) val = sensors.max44009_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic7.publish(str);
            }
          }
          if(config.fd7[0] == 7){
            val = 404.0;
            if(config.fd7[1] == 3) val = sensors.bh1750_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic7.publish(str);
            }
          }
        }
        if(config.fq8){
          if(config.fd8[0] == 0){
            val = 404.0;
            if(config.fd8[1] == 0) val = datas.temp_web;
            else if(config.fd8[1] == 1) val = datas.hum_web;
            else if(config.fd8[1] == 2) val = datas.pres_web;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic8.publish(str);
            }
          }
          if(config.fd8[0] == 1){
            val = 404.0;
            if(config.fd8[1] == 0) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.temp_wsens:4040.0;
            else if(config.fd8[1] == 1) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.hum_wsens:4040.0;
            else if(config.fd8[1] == 2) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.pres_wsens:4040.0;
            else if(config.fd8[1] == 3) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.light_wsens:4040.0;
            else if(config.fd8[1] == 4) val=((now()-datas.time_wsens)<(config.wexpire*60))?datas.wbat_voltage:4040.0;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic8.publish(str);
            }
          }
          if(config.fd8[0] == 2){
            val = 404.0;
            if(config.fd8[1] == 0) val = sensors.bme280_temp;
            else if(config.fd8[1] == 1) val = sensors.bme280_hum;
            else if(config.fd8[1] == 2) val = sensors.bme280_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic8.publish(str);
            }
          }
          if(config.fd8[0] == 3){
            val = 404.0;
            if(config.fd8[1] == 0) val = sensors.bmp180_temp;
            else if(config.fd8[1] == 2) val = sensors.bmp180_pres;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic8.publish(str);
            }
          }
          if(config.fd8[0] == 4){
            val = 404.0;
            if(config.fd8[1] == 0) val = sensors.sht21_temp;
            else if(config.fd8[1] == 1) val = sensors.sht21_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic8.publish(str);
            }
          }
          if(config.fd8[0] == 5){
            val = 404.0;
            if(config.fd8[1] == 0) val = sensors.dht22_temp;
            else if(config.fd8[1] == 1) val = sensors.dht22_hum;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic8.publish(str);
            }
          }
          if(config.fd8[0] == 6){
            val = 404.0;
            if(config.fd8[1] == 3) val = sensors.max44009_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic8.publish(str);
            }
          }
          if(config.fd8[0] == 7){
            val = 404.0;
            if(config.fd8[1] == 3) val = sensors.bh1750_light;
            if(val != 202 and val != 404.0 and val != 4040.0){
              dtostrf(val, 6, 2, str);
              topic8.publish(str);
            }
          }  
        }
        datas.mqtt_sending = false;
      }
    }
  }
}
