#include <Adafruit_PCF8575.h>

/* Example for 1 output LED that is connected from power to the GPIO expander pin #7
 * Note the LEDs must be connected with the CATHODES to the expander, to SINK current!
 * The PCF8575 cannot SOURCE current!
 */

Adafruit_PCF8575 pcf;

void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("Adafruit PCF8575 LED blink test");

  if (!pcf.begin(0x20, &Wire)) {
    Serial.println("Couldn't find PCF8575");
    while (1);
  }
  pcf.pinMode(7, OUTPUT);
}

void loop() {
  pcf.digitalWrite(7, LOW);  // turn LED on by sinking current to ground
  delay(100);
  pcf.digitalWrite(7, HIGH); // turn LED off by turning off sinking transistor
  delay(100);
}
