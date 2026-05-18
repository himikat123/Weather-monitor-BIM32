/****************************************************************
*  SHT21_Demo
*
*  An example sketch that reads the sensor and prints the
*  relative humidity to the serial port
* 
***************************************************************/

#include <Wire.h>
#include "SHT21.h"

SHT21 SHT21;

void setup()
{
  SHT21.begin();
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Humidity(%RH): ");
  Serial.print(SHT21.getHumidity());
  Serial.print("     Temperature(C): ");
  Serial.println(SHT21.getTemperature());
  
  delay(1000);
}
