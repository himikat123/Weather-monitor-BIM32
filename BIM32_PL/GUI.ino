void showForecast(int tempDay, int tempNight, uint8_t icon, uint8_t w_speed, uint8_t w_dir, uint8_t day){
  myNex.writeNum("icon" + String(day) + ".val", icon);                                         // icon
  myNex.writeStr("wn" + String(day) + ".txt", (w_speed > 50) ? "--" : String(w_speed));        // wind speed
  myNex.writeNum("wi" + String(day) + ".val", (w_dir == -1) ? -1 : round(float(w_dir) / 45.0));// wind direction
  if(tempDay > 99 or tempDay < -50) myNex.writeStr("td" + String(day) + ".txt", "--");         // max temp
  else myNex.writeStr("td" + String(day) + ".txt", String(tempDay) + "°");                     // max temp
  if(tempNight > 99 or tempNight < -50) myNex.writeStr("tn" + String(day) + ".txt", "--");     // min temp
  else myNex.writeStr("tn" + String(day) + ".txt", String(tempNight) + "°");                   // min temp
}

void page1_send(void){
  // home temperature and humidity
  int ht = round(datas.temp[1]);
  if(datas.temp[1] < 100.0 and datas.temp[1] > -55.0){
    myNex.writeStr("ht0.txt", "°C");
    myNex.writeStr("ht1.txt", String(ht));
  }
  else{
    myNex.writeStr("ht0.txt", "");
    myNex.writeStr("ht1.txt", "-- ");  
  }
  if(datas.hum[1] <= 100 and datas.hum[1] >= 0){
    myNex.writeStr("hh.txt", String(datas.hum[1]) + "%");
  }
  else myNex.writeStr("hh.txt", "--");
  if(datas.hum[1] == 100) myNex.writeNum("hh.font", 1);
  else myNex.writeNum("hh.font", 2);

  // battery
  if(config.bat_disp == 1){
    if((now() - datas.time_wsens) < (config.wexpire * 60)){
      myNex.writeNum("lbat.val", datas.wbat_voltage >= 0.0 ? datas.wbat_level : -1);
      myNex.writeStr("ubat.txt", String(round(datas.wbat_voltage * 10) / 10, 1));
    }
    else{
      myNex.writeNum("lbat.val", -1);
      myNex.writeStr("ubat.txt", "0");
    }
  }
  if(config.bat_disp == 9 and datas.thng_bat_disp < 20.0){
    datas.bat_voltage = datas.thng_bat_disp;
    float umin = 3.75;
    float umax = 3.9;
    float stp = (umax - umin) / 4;
    if(datas.bat_voltage < (umin + stp)) datas.bat_level = 1;
    else if(datas.bat_voltage < (umin + stp * 2)) datas.bat_level = 2;
    else if(datas.bat_voltage < (umin + stp * 3)) datas.bat_level = 3;
    else datas.bat_level = 4;
    myNex.writeNum("lbat.val", datas.bat_voltage >= 0.0 ? datas.bat_level : -1);
    myNex.writeStr("ubat.txt", String(round(datas.bat_voltage * 10) / 10, 1));
  }
    
  // antenna
  myNex.writeNum("page0.rssi.val", datas.net_connected ? datas.ant : 1);
  myNex.writeStr("page0.ip.txt", WiFi.localIP().toString());
  myNex.writeStr("page0.mac.txt", WiFi.macAddress());
  myNex.writeStr("page0.ssid.txt", WiFi.SSID());
  myNex.writeStr("page0.firmware.txt", String(fw));

  // weather now icon
  myNex.writeNum("isday.val", datas.isDay);
  myNex.writeNum("icon0.val", datas.icon[0]);
  
  // outside temperature
  myNex.writeStr("trm.txt", (datas.temp[0] >= 0.0) ? "+" : "-");
  int ot = round(datas.temp[0]);
  if(datas.temp[0] < 100.0 and datas.temp[0] > -55.0){
    myNex.writeStr("ot0.txt", "°C");
    myNex.writeStr("ot1.txt", String(ot));
  }
  else{
    myNex.writeStr("ot0.txt", "");
    myNex.writeStr("ot1.txt", "-- ");
  }

  // outside humidity
  if(datas.hum[0] <= 100 and datas.hum[0] >= 0){
    myNex.writeStr("oh.txt", String(datas.hum[0]) + "%");
    myNex.writeNum("oh.font", 2);
  }
  else myNex.writeStr("oh.txt", "--");
  if(datas.hum[0] == 100) myNex.writeNum("oh.font", 1);
  else myNex.writeNum("oh.font", 2);

  // outside pressure
  if(datas.pres < 1200 and datas.pres > 500){
    uint16_t pres = round(datas.pres);
    if(pres > 850) pres *= 0.75;
    myNex.writeStr("op.txt", String(pres));
  }
  else myNex.writeStr("op.txt", "--");

  // weather now wind
  uint8_t wsp = round(datas.w_speed[0]);
  if(wsp > 50) myNex.writeStr("wn.txt", "--");
  else myNex.writeStr("wn.txt", String(wsp));
  uint8_t deg = round(float(datas.w_dir[0]) / 45.0);
  if(deg > 7) deg = 0;
  myNex.writeNum("wi0.val", deg);
  
  // weather description
  myNex.writeStr("descr.txt", datas.description);
  
  // updated time
  char recieve_time[40];
  sprintf(recieve_time, 
    "%u:%02u:%02u %02u-%02u-%u",
    hour(datas.w_updated), 
    minute(datas.w_updated),
    second(datas.w_updated),
    day(datas.w_updated),
    month(datas.w_updated),
    year(datas.w_updated)
  );
  if(year(datas.w_updated) > 2020) myNex.writeStr("updtime.txt", recieve_time);

  // weather forecast
  uint8_t wd = weekday() - 1;
  for(uint8_t i = 1; i < 5; i++){
    showForecast(
      round(datas.temp[(i*2)]),
      round(datas.temp[((i*2)+1)]),
      datas.icon[i],
      datas.w_speed[i],
      datas.w_dir[i] > 360 ? 0 : datas.w_dir[i],
      i
    );
    if(++wd > 6) wd = 0;
  }
  
  // CPU temperature
  myNex.writeStr("page0.cputemp.txt", String(round(((temprature_sens_read() - 32) / 1.8) * 10.0) / 10.0, 1));

  // comfort ratio
  if(datas.temp[1] < 100.0 and datas.hum[1] <= 100){
    if(datas.temp[1] >= 22.0 and datas.temp[1] <= 27.0 and datas.hum[1] >= 30 and datas.hum[1] <= 60)
      myNex.writeNum("cfrt.val", 0);
    if(datas.temp[1] > 27.0 and datas.hum[1] >= 30 and datas.hum[1] <= 60)
      myNex.writeNum("cfrt.val", 1);
    if(datas.temp[1] < 22.0 and datas.hum[1] >= 30 and datas.hum[1] <= 60)
      myNex.writeNum("cfrt.val", 2);
    if(datas.temp[1] >= 22.0 and datas.temp[1] <= 27.0 and datas.hum[1] < 30)
      myNex.writeNum("cfrt.val", 3);
    if(datas.temp[1] >= 22.0 and datas.temp[1] <= 27.0 and datas.hum[1] > 60)
      myNex.writeNum("cfrt.val", 4);
    if(datas.temp[1] > 27.0 and datas.hum[1] > 60)
      myNex.writeNum("cfrt.val", 5);
    if(datas.temp[1] > 27.0 and datas.hum[1] < 30)
      myNex.writeNum("cfrt.val", 6);
    if(datas.temp[1] < 22.0 and datas.hum[1] > 60)
      myNex.writeNum("cfrt.val", 7);
    if(datas.temp[1] < 22.0 and datas.hum[1] < 30)
      myNex.writeNum("cfrt.val", 8);
    if(datas.hum[1] > 35) digitalWrite(humidifier, LOW);
    if(datas.hum[1] < 30) digitalWrite(humidifier, HIGH);
  }
  else myNex.writeNum("cfrt.val", -1);

  datas.old_htemp = datas.temp[1];
  datas.old_hhum = datas.hum[1];
  datas.old_ant = datas.ant;
  datas.old_icon = datas.icon[0];
  datas.old_otemp = datas.temp[0];
  datas.old_ohum = datas.hum[0];
  datas.old_opres = datas.pres;
  datas.old_w_speed = datas.w_speed[0];
  datas.old_temp_d = datas.temp[2];
  datas.old_temp_n = datas.temp[3];

  char dat[21] = "";
  char temp[5] = "";
  deg = 0;
  Serial1.flush();
  Serial1.print("page4.data0.txt=\"");
  if(config.provider == 0){
    for(uint8_t i = 0; i < 40; i++){
      int16_t t = round(datas.temp3hourly[i] * 10);
      sprintf(temp, "%04d", t);
      for(uint8_t k = 0; k < 4; k++) dat[k] = temp[k];
      sprintf(temp, "%03d", datas.pres3hourly[i]);
      for(uint8_t k = 0; k < 3; k++) dat[4 + k] = temp[k];
      sprintf(temp, "%02d", datas.icon3hourly[i]);
      for(uint8_t k = 0; k < 2; k++) dat[7 + k] = temp[k];
      sprintf(temp, "%d", weekday(datas.date3hourly[i]) - 1);
      dat[9] = temp[0];
      sprintf(temp, "%02d", day(datas.date3hourly[i]));
      for(uint8_t k = 0; k < 2; k++) dat[10 + k] = temp[k];
      sprintf(temp, "%02d", month(datas.date3hourly[i]) - 1);
      for(uint8_t k = 0; k < 2; k++) dat[12 + k] = temp[k];
      sprintf(temp, "%02d", hour(datas.date3hourly[i]));
      for(uint8_t k = 0; k < 2; k++) dat[14 + k] = temp[k];
      uint8_t wind = round(datas.wspd3hourly[i]);
      sprintf(temp, "%02d", wind);
      for(uint8_t k = 0; k < 2; k++) dat[16 + k] = temp[k];
      deg = round(float(datas.wdeg3hourly[i]) / 45.0);
      if(deg > 7) deg = 0;
      sprintf(temp, "%d", deg);
      dat[18] = temp[0];
      sprintf(temp, "%02d", datas.prec3hourly[i]);
      for(uint8_t k = 0; k < 2; k++) dat[19 + k] = temp[k];
      for(uint8_t k = 0; k < 21; k++) Serial1.print(dat[k]);
    }
  }
  if(config.provider == 1){
    Serial1.print("-1");
  }
  Serial1.print("\"");
  Serial1.write(0xFF);
  Serial1.write(0xFF);
  Serial1.write(0xFF);

  uint8_t fd = 2;
  for(uint8_t i = 0; i < 40; i++){
    if(hour(datas.date3hourly[i]) == 0){
      if(i != 0) myNex.writeNum("page4.day" + String(fd++) + ".val", i);
      if(fd > 4) break;
    }
  }
  myNex.writeNum("upd.val", 1);
}

void page2_send(void){
  myNex.writeStr("d1.txt", code2str(datas.clock_symb[0]));
  myNex.writeStr("d2.txt", code2str(datas.clock_symb[1]));
  myNex.writeStr("d3.txt", code2str(datas.clock_symb[2]));
  myNex.writeStr("d4.txt", code2str(datas.clock_symb[3]));
  myNex.writeNum("color.val", datas.clock_colr);
  myNex.writeNum("upd.val", 1); 
}

void page3_send(void){
  myNex.writeNum("page0.rssi.val", datas.ant);
  myNex.writeStr("page0.cputemp.txt", String(round(((temprature_sens_read() - 32) / 1.8) * 10.0) / 10.0, 1));
  datas.old_ant = datas.ant;
}

void page8_send(void){
  myNex.writeStr("ipd.txt", WiFi.localIP().toString());
  myNex.writeStr("maskd.txt", WiFi.subnetMask().toString());
  myNex.writeStr("gwd.txt", WiFi.gatewayIP().toString());
  myNex.writeStr("ssid.txt", config.ssid);
  myNex.writeStr("pass.txt", config.pass);
  myNex.writeStr("mask.txt", config.mask);
  myNex.writeStr("dns1.txt", config.dns1);
  myNex.writeStr("dns2.txt", config.dns2);
  myNex.writeStr("ip.txt", config.ip);
  myNex.writeStr("gw.txt", config.gw);
  myNex.writeNum("r0.val", !config.type);
  myNex.writeNum("r1.val", config.type);
  myNex.writeNum("upd.val", 1);
}

void page9_send(void){
  for(uint8_t i = 0; i < 4; i++) myNex.writeNum("c" + String(i) + ".val", config.brt == i ? 1 : 0);
  for(uint8_t i = 0; i < 6; i++){
    myNex.writeNum("h" + String(i) + ".val", (i == 1 or i == 4) ? config.brnight : config.brday);
  }
  myNex.writeStr("hd.txt", String(config.hd));
  char minut[3];
  sprintf(minut, "%02d", config.md);
  myNex.writeStr("md.txt", String(minut));
  myNex.writeStr("hn.txt", String(config.hn));
  sprintf(minut, "%02d", config.mn);
  myNex.writeStr("mn.txt", String(minut));
  myNex.writeNum("h2.val", config.sdisp_sensitivity);
  myNex.writeNum("c4.val", (config.light_in == 1) ? 1 : 0);
  myNex.writeNum("c5.val", (config.light_in == 6) ? 1 : 0);
  myNex.writeNum("c6.val", (config.light_in == 7) ? 1 : 0);
  myNex.writeNum("c7.val", (config.light_in == 8) ? 1 : 0);
  myNex.writeNum("c8.val", (config.light_in == 9) ? 1 : 0);
  myNex.writeNum("upd.val", 1);
}

void page10_send(void){
  for(uint8_t i = 0; i < 10; i++){
    myNex.writeNum("to" + String(i) + ".val", config.temp_out == i ? 1 : 0);
    myNex.writeNum("ti" + String(i) + ".val", config.temp_in == i ? 1 : 0);
    myNex.writeNum("ho" + String(i) + ".val", config.hum_out == i ? 1 : 0);
    myNex.writeNum("hi" + String(i) + ".val", config.hum_in == i ? 1 : 0);
    myNex.writeNum("po" + String(i) + ".val", config.pres_out == i ? 1 : 0);
    myNex.writeNum("b" + String(i) + ".val", config.bat_disp == i ? 1 : 0);
  }
  myNex.writeNum("upd.val", 1);
}

void page11_send(void){
  myNex.writeStr("timezone.txt", String(config.utc));
  myNex.writeNum("dlst.val", config.daylight == true ? 1 : 0);
  myNex.writeStr("tp.txt", String(config.ntp));
  myNex.writeNum("upd.val", 1);
}

void page12_send(void){
  myNex.writeStr("city.txt", String(config.city));
  myNex.writeStr("cityid.txt", String(config.cityid));
  myNex.writeStr("lat.txt", String(config.lat));
  myNex.writeStr("lon.txt", String(config.lon));
  myNex.writeStr("apid.txt", String(config.appid));
  myNex.writeStr("key.txt", String(config.appkey));
  if(config.citysearch == 0){
    myNex.writeNum("c0.val", 1);
    myNex.writeNum("c1.val", 0);
    myNex.writeNum("c2.val", 0);
    myNex.writeNum("city.pco", 65535);
  }
  if(config.citysearch == 1){
    myNex.writeNum("c0.val", 0);
    myNex.writeNum("c1.val", 1);
    myNex.writeNum("c2.val", 0);
    myNex.writeNum("cityid.pco", 65535);
  }
  if(config.citysearch == 2){
    myNex.writeNum("c0.val", 0);
    myNex.writeNum("c1.val", 0);
    myNex.writeNum("c2.val", 1);
    myNex.writeNum("lat.pco", 65535);
    myNex.writeNum("lon.pco", 65535);
    myNex.writeNum("t5.pco", 65535);
    myNex.writeNum("t6.pco", 65535);
  }
  if(config.provider == 0){
    myNex.writeNum("c3.val", 1);
    myNex.writeNum("c4.val", 0);
    myNex.writeStr("t9.txt", "APPID");
  }
  if(config.provider == 1){
    myNex.writeNum("c3.val", 0);
    myNex.writeNum("c4.val", 1);
    myNex.writeStr("t9.txt", "KEY");
  }
  myNex.writeNum("upd.val", 1);
}

void page13_send(void){
  myNex.writeStr("bmet.txt", String(sensors.bme280_temp_corr, 1));
  myNex.writeStr("bmpt.txt", String(sensors.bmp180_temp_corr, 1));
  myNex.writeStr("shtt.txt", String(sensors.sht21_temp_corr, 1));
  myNex.writeStr("dhtt.txt", String(sensors.dht22_temp_corr, 1));
  myNex.writeStr("bmeh.txt", String(sensors.bme280_hum_corr));
  myNex.writeStr("shth.txt", String(sensors.sht21_hum_corr));
  myNex.writeStr("dhth.txt", String(sensors.dht22_hum_corr));
  myNex.writeStr("bmep.txt", String(sensors.bme280_pres_corr));
  myNex.writeStr("bmpp.txt", String(sensors.bmp180_pres_corr));
  myNex.writeStr("max.txt", String(sensors.max44009_light_corr, 1));
  myNex.writeStr("bh.txt", String(sensors.bh1750_light_corr, 1));
  myNex.writeNum("upd.val", 1);
}

void sens_data_send(void){
  myNex.writeStr("bme280t.txt", sensors.bme280_temp > 100.0 ? "--" : String(round(sensors.bme280_temp * 10.0) / 10.0, 1));
  myNex.writeStr("bme280h.txt", sensors.bme280_hum > 100 ? "--" : String(round(sensors.bme280_hum * 10.0) / 10.0, 1));
  myNex.writeStr("bme280p.txt", sensors.bme280_pres > 1500 ? "--" : String(round(sensors.bme280_pres * 10.0) / 10.0, 1));
  myNex.writeStr("bmp180t.txt", sensors.bmp180_temp > 100.0 ? "--" : String(round(sensors.bmp180_temp * 10.0) / 10.0, 1));
  myNex.writeStr("bmp180p.txt", sensors.bmp180_pres > 1500 ? "--" : String(round(sensors.bmp180_pres * 10.0) / 10.0, 1));
  myNex.writeStr("sht21t.txt", sensors.sht21_temp > 100.0 ? "--" : String(round(sensors.sht21_temp * 10.0) / 10.0, 1));
  myNex.writeStr("sht21h.txt", sensors.sht21_hum > 100 ? "--" : String(round(sensors.sht21_hum * 10.0) / 10.0, 1));
  myNex.writeStr("dht22t.txt", sensors.dht22_temp > 100.0 ? "--" : String(round(sensors.dht22_temp * 10.0) / 10.0, 1));
  myNex.writeStr("dht22h.txt", sensors.dht22_hum > 100 ? "--" : String(round(sensors.dht22_hum * 10.0) / 10.0, 1));
  myNex.writeStr("max44009.txt", sensors.max44009_light < 0 ? "--" : String(round(sensors.max44009_light * 10.0) / 10.0, 1));
  myNex.writeStr("bh1750.txt", sensors.bh1750_light < 0 ? "--" : String(round(sensors.bh1750_light * 10.0) / 10.0, 1));
}

void page14_send(void){
  myNex.writeStr("wtempc.txt", String(config.wsens_temp_corr, 1));
  myNex.writeStr("whumc.txt", String(config.wsens_hum_corr));
  myNex.writeStr("wpresc.txt", String(config.wsens_pres_corr));
  myNex.writeStr("wlightc.txt", String(config.wsens_light_corr, 1));
  myNex.writeNum("wbatc.val", 300 - config.bat_k);
  myNex.writeNum("bat0.val", config.bat_type == 0 ? 1 : 0);
  myNex.writeNum("bat1.val", config.bat_type == 1 ? 1 : 0);
  myNex.writeStr("chnum.txt", String(config.wsens_channel));
  myNex.writeStr("wexpire.txt", String(config.wexpire));
  myNex.writeNum("upd.val", 1);
}

void wsens_data_send(void){
  char recieve_time[40];
  sprintf(recieve_time, 
    "%u:%02u:%02u %02u-%02u-%u",
    hour(datas.time_wsens), 
    minute(datas.time_wsens),
    second(datas.time_wsens),
    day(datas.time_wsens),
    month(datas.time_wsens),
    year(datas.time_wsens)
  );
  myNex.writeStr("time.txt", datas.time_wsens < 1000 ? "---" : recieve_time);
  myNex.writeStr("wtemp.txt", datas.temp_wsens > 100.0 ? "--" : String(round(datas.temp_wsens * 10.0) / 10.0, 1));
  myNex.writeStr("whum.txt", datas.hum_wsens > 100 ? "--" : String(round(datas.hum_wsens * 10.0) / 10.0, 1));
  myNex.writeStr("wpres.txt", datas.pres_wsens > 1500 ? "--" : String(round(datas.pres_wsens * 10.0) / 10.0, 1));
  myNex.writeStr("wlight.txt", datas.light_wsens < 0 ? "--" : String(round(datas.light_wsens * 10.0) / 10.0, 1));
  myNex.writeStr("wbat.txt", datas.bat_voltage < 0 ? "--" : String(datas.bat_voltage));
  myNex.writeStr("wtempt.txt", String(datas.wtemp_sens));
  myNex.writeStr("whumt.txt", String(datas.whum_sens));
  myNex.writeStr("wprest.txt", String(datas.wpres_sens));
  myNex.writeStr("wlightt.txt", String(datas.wlight_sens));
}

void page15_send(void){
  myNex.writeStr("tupd.txt", String(config.tupd));
  myNex.writeNum("mqttsend.val", config.mqttsend ? 1 : 0);
  myNex.writeNum("thngsend.val", config.thngsend ? 1 : 0);
  myNex.writeNum("nrdmsend.val", config.nrdmsend ? 1 : 0);
  String mac = "BIM" + WiFi.macAddress();
  mac.replace(":", "");
  myNex.writeStr("monmac.txt", mac);
  myNex.writeNum("upd.val", 1);
}

void page16_send(void){
  uint16_t color[5] = {65504, 2047, 63519, 64512, 2016};
  for(uint8_t i = 0; i < 8; i++){
    myNex.writeNum("f1" + String(i) + ".val", config.fd1[0] == i ? 1 : 0);
    if(i < 6 and config.fd1[0] == i) myNex.writeNum("f1" + String(i) + ".pco", color[config.fd1[1]]);
    myNex.writeNum("f2" + String(i) + ".val", config.fd2[0] == i ? 1 : 0);
    if(i < 6 and config.fd2[0] == i) myNex.writeNum("f2" + String(i) + ".pco", color[config.fd2[1]]);
    myNex.writeNum("f3" + String(i) + ".val", config.fd3[0] == i ? 1 : 0);
    if(i < 6 and config.fd3[0] == i) myNex.writeNum("f3" + String(i) + ".pco", color[config.fd3[1]]);
    myNex.writeNum("f4" + String(i) + ".val", config.fd4[0] == i ? 1 : 0);
    if(i < 6 and config.fd4[0] == i) myNex.writeNum("f4" + String(i) + ".pco", color[config.fd4[1]]);
    myNex.writeNum("f5" + String(i) + ".val", config.fd5[0] == i ? 1 : 0);
    if(i < 6 and config.fd5[0] == i) myNex.writeNum("f5" + String(i) + ".pco", color[config.fd5[1]]);
    myNex.writeNum("f6" + String(i) + ".val", config.fd6[0] == i ? 1 : 0);
    if(i < 6 and config.fd6[0] == i) myNex.writeNum("f6" + String(i) + ".pco", color[config.fd6[1]]);
    myNex.writeNum("f7" + String(i) + ".val", config.fd7[0] == i ? 1 : 0);
    if(i < 6 and config.fd7[0] == i) myNex.writeNum("f7" + String(i) + ".pco", color[config.fd7[1]]);
    myNex.writeNum("f8" + String(i) + ".val", config.fd8[0] == i ? 1 : 0);
    if(i < 6 and config.fd8[0] == i) myNex.writeNum("f8" + String(i) + ".pco", color[config.fd8[1]]);
  }
  myNex.writeNum("upd.val", 1);
}

void page17_send(void){
  uint16_t color[5] = {65504, 2047, 63519, 64512, 2016};
  for(uint8_t i = 0; i < 8; i++){
    myNex.writeNum("f1" + String(i) + ".val", config.f1[0] == i ? 1 : 0);
    if(i < 6 and config.f1[0] == i) myNex.writeNum("f1" + String(i) + ".pco", color[config.f1[1]]);
    myNex.writeNum("f2" + String(i) + ".val", config.f2[0] == i ? 1 : 0);
    if(i < 6 and config.f2[0] == i) myNex.writeNum("f2" + String(i) + ".pco", color[config.f2[1]]);
    myNex.writeNum("f3" + String(i) + ".val", config.f3[0] == i ? 1 : 0);
    if(i < 6 and config.f3[0] == i) myNex.writeNum("f3" + String(i) + ".pco", color[config.f3[1]]);
    myNex.writeNum("f4" + String(i) + ".val", config.f4[0] == i ? 1 : 0);
    if(i < 6 and config.f4[0] == i) myNex.writeNum("f4" + String(i) + ".pco", color[config.f4[1]]);
    myNex.writeNum("f5" + String(i) + ".val", config.f5[0] == i ? 1 : 0);
    if(i < 6 and config.f5[0] == i) myNex.writeNum("f5" + String(i) + ".pco", color[config.f5[1]]);
    myNex.writeNum("f6" + String(i) + ".val", config.f6[0] == i ? 1 : 0);
    if(i < 6 and config.f6[0] == i) myNex.writeNum("f6" + String(i) + ".pco", color[config.f6[1]]);
    myNex.writeNum("f7" + String(i) + ".val", config.f7[0] == i ? 1 : 0);
    if(i < 6 and config.f7[0] == i) myNex.writeNum("f7" + String(i) + ".pco", color[config.f7[1]]);
    myNex.writeNum("f8" + String(i) + ".val", config.f8[0] == i ? 1 : 0);
    if(i < 6 and config.f8[0] == i) myNex.writeNum("f8" + String(i) + ".pco", color[config.f8[1]]);
  }
  myNex.writeNum("upd.val", 1);
}

void page18_send(void){
  for(uint8_t i = 0; i < 10; i++){
    if(i < 6){
      myNex.writeNum("to" + String(i) + ".val", config.nrd_tout == i ? 1 : 0);
      myNex.writeNum("ti" + String(i) + ".val", config.nrd_tin == i ? 1 : 0);
    }
    if(i < 6 and i != 3){
      myNex.writeNum("ho" + String(i) + ".val", config.nrd_hout == i ? 1 : 0);
      myNex.writeNum("hi" + String(i) + ".val", config.nrd_hin == i ? 1 : 0);
    }
    if(i < 4) myNex.writeNum("po" + String(i) + ".val", config.nrd_pout == i ? 1 : 0);
    if(i == 1 or i > 7) myNex.writeNum("li" + String(i) + ".val", config.nrd_lin == i ? 1 : 0);
  }
  myNex.writeStr("lat.txt", String(config.lat));
  myNex.writeStr("lon.txt", String(config.lon));
  myNex.writeNum("upd.val", 1);
}

void page19_send(void){
  myNex.writeStr("mqttServer.txt", String(config.mqttServer));
  myNex.writeStr("mqttPort.txt", String(config.mqttPort));
  myNex.writeStr("mqttLogin.txt", String(config.mqttLogin));
  myNex.writeStr("mqttPass.txt", String(config.mqttPass));
  myNex.writeStr("mqttF1.txt", String(config.mqttF1));
  myNex.writeStr("mqttF2.txt", String(config.mqttF2));
  myNex.writeStr("mqttF3.txt", String(config.mqttF3));
  myNex.writeStr("mqttF4.txt", String(config.mqttF4));
  myNex.writeStr("mqttF5.txt", String(config.mqttF5));
  myNex.writeStr("mqttF6.txt", String(config.mqttF6));
  myNex.writeStr("mqttF7.txt", String(config.mqttF7));
  myNex.writeStr("mqttF8.txt", String(config.mqttF8));
  myNex.writeNum("f1.val", config.fq1 ? 1 : 0);
  myNex.writeNum("f2.val", config.fq2 ? 1 : 0);
  myNex.writeNum("f3.val", config.fq3 ? 1 : 0);
  myNex.writeNum("f4.val", config.fq4 ? 1 : 0);
  myNex.writeNum("f5.val", config.fq5 ? 1 : 0);
  myNex.writeNum("f6.val", config.fq6 ? 1 : 0);
  myNex.writeNum("f7.val", config.fq7 ? 1 : 0);
  myNex.writeNum("f8.val", config.fq8 ? 1 : 0);
  myNex.writeNum("upd.val", 1);
}

void page20_send(void){
  myNex.writeStr("rdkey.txt", String(config.rdkey));
  myNex.writeStr("wrkey.txt", String(config.wrkey));
  myNex.writeStr("chid.txt", String(config.chid));
  myNex.writeNum("upd.val", 1);
}

void page21_send(void){
  myNex.writeNum("mqttrcv.val", config.mqttrcv ? 1 : 0);
  myNex.writeNum("thngrcv.val", config.thngrcv ? 1 : 0);
  myNex.writeNum("upd.val", 1);
}

void page22_send(void){
  for(uint8_t i = 0; i < 8; i++){
    myNex.writeNum("f" + String(i) + "0.val", config.mto == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "1.val", config.mho == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "2.val", config.mpo == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "3.val", config.mti == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "4.val", config.mhi == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "5.val", config.mli == i ? 1 : 0);
  }
  myNex.writeNum("upd.val", 1);
}

void page23_send(void){
  for(uint8_t i = 1; i < 9; i++){
    myNex.writeNum("f" + String(i) + "0.val", config.tto == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "1.val", config.tho == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "2.val", config.tpo == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "3.val", config.tti == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "4.val", config.thi == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "5.val", config.tli == i ? 1 : 0);
    myNex.writeNum("f" + String(i) + "6.val", config.tbt == i ? 1 : 0);
  }
  myNex.writeNum("upd.val", 1);
}

void page24_send(void){
  uint8_t k = 0;
  for(uint8_t i = 0; i < 42; i++){
    uint8_t ind = floor(i / 7);
    myNex.writeNum("f" + String(i) + ".val", config.dt[ind] == k + 1 ? 1 : 0);
    if(config.dt[ind] == k + 1){
      myNex.writeNum("f" + String(i) + ".pco", config.dc[ind]);
      if(config.ds[ind] != "0") myNex.writeStr("s" + String(i) + ".txt", config.ds[ind]);
    }
    if(++k > 6) k = 0;
  }
  for(uint8_t k = 0; k < 6; k++){
    myNex.writeStr("t" + String(k + 1) + ".txt", String(config.dp[k]));
  }
  myNex.writeNum("upd.val", 1);
}

void page30_send(void){
  for(uint8_t i = 0; i < 4; i++) myNex.writeNum("c" + String(i) + ".val", config.ws_brt == i ? 1 : 0);
  for(uint8_t i = 0; i < 6; i++){
    myNex.writeNum("h" + String(i) + ".val", (i == 1 or i == 4) ? config.ws_brightd : config.ws_brightn);
  }
  myNex.writeStr("hd.txt", String(config.ws_hd));
  char minut[3];
  sprintf(minut, "%02d", config.ws_md);
  myNex.writeStr("md.txt", String(minut));
  myNex.writeStr("hn.txt", String(config.ws_hn));
  sprintf(minut, "%02d", config.ws_mn);
  myNex.writeStr("mn.txt", String(minut));
  myNex.writeNum("h2.val", config.ws_sdisp_sensitivity);
  myNex.writeNum("c4.val", (config.ws_light_in == 1) ? 1 : 0);
  myNex.writeNum("c5.val", (config.ws_light_in == 6) ? 1 : 0);
  myNex.writeNum("c6.val", (config.ws_light_in == 7) ? 1 : 0);
  myNex.writeNum("c7.val", (config.ws_light_in == 8) ? 1 : 0);
  myNex.writeNum("c8.val", (config.ws_light_in == 9) ? 1 : 0);
  myNex.writeNum("upd.val", 1);
}

void page31_send(void){
  myNex.writeNum("n0.val", config.disp_autooff);
  myNex.writeNum("upd.val", 1);
}
