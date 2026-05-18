#include <Adafruit_PCF8575.h>

/* Example for 16 input buttons that are connected from the GPIO expander pins to ground.
 * Note the buttons must be connected with the other side of the switch to GROUND. There is
 * a built in pull-up 'resistor' on each input, but no pull-down resistor capability.
 */

Adafruit_PCF8575 pcf;

void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("Adafruit PCF8575 button read test");

  if (!pcf.begin(0x20, &Wire)) {
    Serial.println("Couldn't find PCF8575");
    while (1);
  }
  for (uint8_t p=0; p<16; p++) {
    pcf.pinMode(p, INPUT_PULLUP);
  }
}

void loop() {
  for (uint8_t p=0; p<16; p++) {
    if (! pcf.digitalRead(p)) {
      Serial.print("Button on GPIO #");
      Serial.print(p);
      Serial.println(" pressed!");
    }
  }
  delay(10); // a short debounce delay
}
