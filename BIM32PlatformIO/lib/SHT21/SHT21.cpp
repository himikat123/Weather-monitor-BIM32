/*
  SHT21 - Library for ESP8266 and Arduino for the Sensirion Temperature and Humidity sensor

  Created by Markus Ulsass, Hamburg, Germany
  github@tradewire.de
  23-5-2016
  https://github.com/markbeee/SHT21
  
  With credits to:
  
  HTU21D Humidity Sensor Library
  By: Nathan Seidle
  SparkFun Electronics
  Date: September 22nd, 2013
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

*/

#include <Wire.h>
#include "SHT21.h"

SHT21::SHT21 () {}

void SHT21::begin(void){
		
		Wire.begin();  
				
	}
	
float SHT21::getHumidity(void)

{
	return (-6.0 + 125.0 / 65536.0 * (float)(readSHT21(TRIGGER_HUMD_MEASURE_NOHOLD)));
}

float SHT21::getTemperature(void)
{
	return (-46.85 + 175.72 / 65536.0 * (float)(readSHT21(TRIGGER_TEMP_MEASURE_NOHOLD)));
}

uint16_t SHT21::readSHT21(uint8_t command)
{
    uint16_t result;

    Wire.beginTransmission(SHT21_ADDRESS);	
    Wire.write(command);					
    Wire.endTransmission();
	delay(100);

    Wire.requestFrom(SHT21_ADDRESS, 3);
    while(Wire.available() < 3) {
      delay(1);
    }

    // return result
    result = ((Wire.read()) << 8);
    result += Wire.read();
	result &= ~0x0003;   // clear two low bits (status bits)
    return result;
}