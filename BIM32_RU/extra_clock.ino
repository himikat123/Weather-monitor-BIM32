String code2str(uint8_t code){
  String str = " ";
  switch(code){
    case 0  : str = "0"; break;
    case 1  : str = "1"; break;
    case 2  : str = "2"; break;
    case 3  : str = "3"; break;
    case 4  : str = "4"; break;
    case 5  : str = "5"; break;
    case 6  : str = "6"; break;
    case 7  : str = "7"; break;
    case 8  : str = "8"; break;
    case 9  : str = "9"; break;
    case 10 : str = " "; break;
    case 11 : str = "-"; break;
    case 12 : str = "°"; break;
    case 13 : str = "C"; break;
    case 14 : str = "H"; break;
    case 15 : str = "P"; break;
    case 16 : str = "T"; break;
    default : str = " "; break;
  }
  return str;
}

bool get_clock(bool dots){
  datas.clock_dig[0] = hour() / 10; 
  datas.clock_dig[1] = hour() % 10; 
  datas.clock_dig[2] = minute() / 10; 
  datas.clock_dig[3] = minute() % 10;
  if(datas.clock_dig[0] == 0) datas.clock_dig[0] = 10;
  return !dots;
}

bool get_date(){
  datas.clock_dig[0] = day() / 10; 
  datas.clock_dig[1] = day() % 10; 
  datas.clock_dig[2] = month() / 10; 
  datas.clock_dig[3] = month() % 10;
  if(datas.clock_dig[0] == 0) datas.clock_dig[0] = 10;
  return 0;
}

bool get_temp(float t){
  int buf = round(t);
  if(buf > 99){
    datas.clock_dig[0] = datas.clock_dig[1] = 11; 
    datas.clock_dig[2] = 12; 
    datas.clock_dig[3] = 13;
  }
  else{
    if(buf >= 0){
      datas.clock_dig[0] = buf < 10 ? 10 : buf / 10; 
      datas.clock_dig[1] = buf < 10 ? buf : buf % 10; 
      datas.clock_dig[2] = 12; 
      datas.clock_dig[3] = 13;
    }
    else{
      buf = abs(buf);
      datas.clock_dig[0] = 11; 
      datas.clock_dig[1] = buf < 10 ? buf : buf / 10; 
      datas.clock_dig[2] = buf < 10 ? 12 : buf % 10; 
      datas.clock_dig[3] = buf < 10 ? 13 : 12;
    }
  }
  return 0;
}

bool get_hum(float h){
  uint8_t buf = round(h);
  datas.clock_dig[3] = 14;
  if(buf > 100){
    datas.clock_dig[0] = datas.clock_dig[1] = 11;
    datas.clock_dig[2] = 10;
  }
  else{
    if(buf < 10) datas.clock_dig[1] = buf;
    else if(buf < 100){
      datas.clock_dig[0] = buf / 10; 
      datas.clock_dig[1] = buf % 10;
      datas.clock_dig[2] = 10;
    }
    else{
      datas.clock_dig[0] = 1; 
      datas.clock_dig[1] = 0; 
      datas.clock_dig[2] = 0;
    }
  }
  return 0;
}

bool get_pres(float p){
  uint16_t buf = round(p);
  datas.clock_dig[3] = 15;
  if(buf > 1200){
    datas.clock_dig[0] = datas.clock_dig[1] = datas.clock_dig[2] = 11;
  }
  else{
    datas.clock_dig[0] = buf / 100; 
    datas.clock_dig[1] = buf % 100 / 10; 
    datas.clock_dig[2] = buf % 10;
  }
  return 0;
}

void TaskWS2812B(void *pvParameters){
  (void) pvParameters;
  strip.Begin();
  strip.Show();
  bool clock_pixels[30];
  uint8_t symb[16] = {0x3F,0x0C,0x76,0x5E,0x4D,0x5B,0x7B,0x0E,0x7F,0x5F,0x00,0x40,0x47,0x33,0x6D,0x67};//0123456789 -°CHP
  uint32_t prev_millis = 0;
  uint8_t snum = 0;
  bool dotts = false;
  for(uint8_t i=0; i<30; i++){
    clock_pixels[i] = 0; 
  }
  while(1){
    int bright = datas.bright_clock;
    if(bright < 1) bright = 1;
    if(bright > 100) bright = 100;
    RgbColor red(bright, 0, 0);
    RgbColor yellow(bright, bright, 0);
    RgbColor green(0, bright, 0);
    RgbColor cyan(0, bright, bright);
    RgbColor blue(0, 0, bright);
    RgbColor purple(bright, 0, bright);
    RgbColor white(bright);
    RgbColor black(0);
    if((millis() - prev_millis >= config.dp[snum] * 1000) or (config.dp[snum] == 0)){
      snum++;
      for(uint8_t i=snum; i<6; i++){
        if(config.dp[snum] == 0) snum++;
        else break;
      }
      if(snum > 5) snum = 0;
      prev_millis = millis();
    }
    
    if(config.dp[snum] > 0){
      switch(config.dt[snum]){
        case 1: 
          if(String(config.ds[snum]) == "C") dotts = get_clock(dotts);
          if(String(config.ds[snum]) == "D") dotts = get_date();
          break;
        case 2:
          if(String(config.ds[snum]) == "T") dotts = get_temp(datas.temp_web);
          if(String(config.ds[snum]) == "H") dotts = get_hum((float)datas.hum_web);
          if(String(config.ds[snum]) == "P") dotts = get_pres((float)datas.pres_web);
          break;
        case 3:
          if(String(config.ds[snum]) == "T") dotts = get_temp(datas.temp_wsens);
          if(String(config.ds[snum]) == "H") dotts = get_hum((float)datas.hum_wsens);
          if(String(config.ds[snum]) == "P") dotts = get_pres((float)datas.pres_wsens);
          break;
        case 4:
          if(String(config.ds[snum]) == "T") dotts = get_temp(sensors.bme280_temp);
          if(String(config.ds[snum]) == "H") dotts = get_hum((float)sensors.bme280_hum);
          if(String(config.ds[snum]) == "P") dotts = get_pres((float)sensors.bme280_pres);
          break;
        case 5:
          if(String(config.ds[snum]) == "T") dotts = get_temp(sensors.bmp180_temp);
          if(String(config.ds[snum]) == "P") dotts = get_pres((float)sensors.bmp180_pres);
          break;
        case 6:
          if(String(config.ds[snum]) == "T") dotts = get_temp(sensors.sht21_temp);
          if(String(config.ds[snum]) == "H") dotts = get_hum((float)sensors.sht21_hum);
          break;
        case 7:
          if(String(config.ds[snum]) == "T") dotts = get_temp(sensors.dht22_temp);
          if(String(config.ds[snum]) == "H") dotts = get_hum((float)sensors.dht22_hum);
          break;
        default:;; 
      }
      clock_pixels[14] = clock_pixels[15] = dotts;
    }

    if(config.dt[snum] == 1 && String(config.ds[snum]) == "C"){
      for(uint8_t i=0; i<4; i++) datas.clock_symb[i] = 16;
    }
    else{
      for(uint8_t i=0; i<4; i++) datas.clock_symb[i] = datas.clock_dig[i];
    }
    datas.clock_colr = config.dc[snum];

    for(uint8_t i=0; i<7; i++){
      clock_pixels[i] = bitRead(symb[datas.clock_dig[0]], i) ? 1 : 0;
      clock_pixels[i + 7] = bitRead(symb[datas.clock_dig[1]], i) ? 1 : 0;
      clock_pixels[i + 16] = bitRead(symb[datas.clock_dig[2]], i) ? 1 : 0;
      clock_pixels[i + 23] = bitRead(symb[datas.clock_dig[3]], i) ? 1 : 0;
    }
    
    for(uint8_t i=0; i<30; i++){
      if(clock_pixels[i]){
        switch(config.dc[snum]){
          case 65535: strip.SetPixelColor(i, white);  break;
          case 63488: strip.SetPixelColor(i, red);    break;
          case 2016:  strip.SetPixelColor(i, green);  break;
          case 31:    strip.SetPixelColor(i, blue);   break;
          case 65504: strip.SetPixelColor(i, yellow); break;
          case 2047:  strip.SetPixelColor(i, cyan);   break;
          case 63519: strip.SetPixelColor(i, purple); break;
          default: ; break;
        }
      }
      else strip.SetPixelColor(i, black);
    }
    strip.Show();
    vTaskDelay(500);
  }
}
