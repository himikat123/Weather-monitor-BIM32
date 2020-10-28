bool get_clock(bool dots){
  datas.clock_dig[0] = hour() / 10; 
  datas.clock_dig[1] = hour() % 10; 
  datas.clock_dig[2] = minute() / 10; 
  datas.clock_dig[3] = minute() % 10;
  if(datas.clock_dig[0] == 0) datas.clock_dig[0] = 10;
  return !dots;
}

bool get_temp(){
  int buf = round(datas.temp[1]);
  if(buf >= 0){
    datas.clock_dig[0] = buf<10 ? 10 : buf/10; 
    datas.clock_dig[1] = buf<10 ? buf : buf % 10; 
    datas.clock_dig[2] = 12; 
    datas.clock_dig[3] = 13;
  }
  else{
    datas.clock_dig[0] = 11; 
    datas.clock_dig[1] = buf>-10 ? buf : buf / 10; 
    datas.clock_dig[2] = buf>-10 ? 12 : buf % 10; 
    datas.clock_dig[3] = buf>-10 ? 13 : 12;
  }
  return 0;
}

bool get_hum(){
  uint8_t buf = round(datas.hum[1]);
  datas.clock_dig[3] = 14;
  if(buf<10) datas.clock_dig[1] = buf;
  else if(buf<100){
    datas.clock_dig[0] = buf/10; 
    datas.clock_dig[1] = buf%10;
    datas.clock_dig[2] = 10;
  }
  else{
    datas.clock_dig[0] = 1; 
    datas.clock_dig[1] = 0; 
    datas.clock_dig[2] = 0;
  }
  return 0;
}

bool get_pres(){
  uint16_t buf = round(datas.pres);
  datas.clock_dig[0] = buf/100; 
  datas.clock_dig[1] = buf%100/10; 
  datas.clock_dig[2] = buf%10;
  datas.clock_dig[3] = 15;
  return 0;
}

void TaskWS2812B(void *pvParameters){
  (void) pvParameters;
  strip.Begin();
  strip.Show();
  bool clock_pixels[30];
  uint8_t symb[16] = {0x3F,0x0C,0x76,0x5E,0x4D,0x5B,0x7B,0x0E,0x7F,0x5F,0x00,0x40,0x47,0x33,0x6D,0x67};//0123456789 -°CHP
  uint8_t itm = 0;
  uint32_t prev_millis = 0;
  for(uint8_t i=0; i<30; i++){
    clock_pixels[i] = 0; 
  }
  while(1){
    clock_pixels[14] = clock_pixels[15];
    
    for(uint8_t i=0; i<7; i++){
      clock_pixels[i] = bitRead(symb[datas.clock_dig[0]],i) ? 1 : 0;
      clock_pixels[i+7] = bitRead(symb[datas.clock_dig[1]],i) ? 1 : 0;
      clock_pixels[i+16] = bitRead(symb[datas.clock_dig[2]],i) ? 1 : 0;
      clock_pixels[i+23] = bitRead(symb[datas.clock_dig[3]],i) ? 1 : 0; 
    }
    
    for(uint8_t i=0; i<30; i++){
      if(clock_pixels[i]) strip.SetPixelColor(i, white);
      else strip.SetPixelColor(i, black);
    }
    strip.Show();

    if(millis()-prev_millis > 2000){
      if(itm >= 7) itm = 0;
      else itm++;
      prev_millis = millis();
    }
        
    vTaskDelay(500);
  }
}
