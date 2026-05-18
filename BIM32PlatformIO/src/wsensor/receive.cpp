#include <Arduino.h>
#include <ArduinoJson.h> // v7.0.3 https://arduinojson.org/?utm_source=meta&utm_medium=library.properties

#include "./wirelessSensor.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"
#include "../pinout.hpp"

void WirelessSensor::receive() {
    String wsensorStr = "";
    char lastChar = _receivedData[_receivedIndex];
    char last_char = _receivedData[_receivedIndex - 1];
    if(lastChar == '\n' or lastChar == '}' or last_char == '\n' or last_char == '}') {
        _receivedIndex = 0;
        Serial.println(SEPARATOR);
        Serial.println("Wireless sensor receive... ");

        String received = String(_receivedData);
        int index = received.indexOf('\n');
        wsensorStr = received.substring(0, index);
        Serial.println(wsensorStr);
        for(uint8_t i=0; i<3; i++) {
            int startJson = wsensorStr.indexOf("{");
            int endJson = wsensorStr.indexOf("}");
            if(startJson != -1 and endJson != -1) {
                JsonDocument root;
                String js = wsensorStr.substring(startJson, endJson + 1);
                wsensorStr = wsensorStr.substring(endJson + 1);
                DeserializationError error = deserializeJson(root, js);
                if(error) {
                    Serial.println("Wireless sensor deserialization error");
                    return;
                }
                int number = root["num"];
                if(number >= 0 and number < WSENSORS) {
                    state.wsensor.time[number] = now();
                    // temperature
                    state.wsensor.temp.data[0][number] = root["t"] | 40400.0;
                    strlcpy(state.wsensor.temp.name[0][number], root["s"] | "", sizeof(state.wsensor.temp.name[0][number]));
                    for(int i=1; i<5; i++) {
                        state.wsensor.temp.data[i][number] = root["ds"][i - 1] | 40400.0;
                        strlcpy(state.wsensor.temp.name[i][number], "DS18B20", sizeof(state.wsensor.temp.name[i][number]));
                    }
                    // humidity
                    state.wsensor.hum.data[number] = root["h"] | 40400.0;
                    strlcpy(state.wsensor.hum.name[number], root["s"] | "", sizeof(state.wsensor.hum.name[number]));
                    // pressure
                    state.wsensor.pres.data[number] = root["p"] | 40400.0;
                    strlcpy(state.wsensor.pres.name[number], root["s"] | "", sizeof(state.wsensor.pres.name[number]));
                    // wind
                    state.wsensor.wind.speed.data[number] = root["wind"][0] | -1;
                    strlcpy(state.wsensor.wind.speed.name[number], "RS485", sizeof(state.wsensor.wind.speed.name[number]));
                    state.wsensor.wind.dir.data[number] = root["wind"][1] | -1;
                    strlcpy(state.wsensor.wind.dir.name[number], "RS485", sizeof(state.wsensor.wind.dir.name[number]));
                    // ambient light
                    state.wsensor.light.data[number] = root["l"] | -1.0;
                    strlcpy(state.wsensor.light.name[number], root["a"] | "", sizeof(state.wsensor.light.name[number]));
                    // PZEM-004t
                    state.wsensor.voltage.data[number] = root["pzem"][0] | -1.0;
                    strlcpy(state.wsensor.voltage.name[number], "PZEM-004t", sizeof(state.wsensor.voltage.name[number]));
                    state.wsensor.current.data[number] = root["pzem"][1] | -1.0;
                    strlcpy(state.wsensor.current.name[number], "PZEM-004t", sizeof(state.wsensor.current.name[number]));
                    state.wsensor.power.data[number] = root["pzem"][2] | -1.0;
                    strlcpy(state.wsensor.power.name[number], "PZEM-004t", sizeof(state.wsensor.power.name[number]));
                    state.wsensor.energy.data[number] = root["pzem"][3] | -1.0;
                    strlcpy(state.wsensor.energy.name[number], "PZEM-004t", sizeof(state.wsensor.energy.name[number]));
                    state.wsensor.freq.data[number] = root["pzem"][4] | -1.0;
                    strlcpy(state.wsensor.freq.name[number], "PZEM-004t", sizeof(state.wsensor.freq.name[number]));
                    // Senseair S8
                    state.wsensor.co2.data[number] = root["s8"] | -1.0;
                    strlcpy(state.wsensor.co2.name[number], "Senseair S8", sizeof(state.wsensor.co2.name[number]));
                    // Battery
                    state.wsensor.battery.adc[number] = root["b"] | -1;
                    state.wsensor.battery.voltage[number] = (float)state.wsensor.battery.adc[number] / (300.0 - config.wsensor.batK(number));
                    if(state.wsensor.battery.voltage[number] > 0.0) {
                        float umin = 3.75;
                        float umax = 3.9;
                        if(config.wsensor.batType(number) == 0) umax = 4.5;
                        float stp = (umax - umin) / 4;
                        if(state.wsensor.battery.voltage[number] < (umin + stp)) state.wsensor.battery.level[number] = 1;
                        else if(state.wsensor.battery.voltage[number] < (umin + stp * 2)) state.wsensor.battery.level[number] = 2;
                        else if(state.wsensor.battery.voltage[number] < (umin + stp * 3)) state.wsensor.battery.level[number] = 3;
                        else state.wsensor.battery.level[number] = 4;
                        state.wsensor.battery.percentage[number] = (state.wsensor.battery.voltage[number] - umin) * 100.0 / (umax - umin); 
                        if(state.wsensor.battery.percentage[number] > 100.0) state.wsensor.battery.percentage[number] = 100.0;
                    }

                    Serial.printf("Sensor %d updated", number);
                    time_t updTime = state.wsensor.time[number];
                    if(updTime > 0) Serial.printf(" at: %d:%02d:%02d\r\n", hour(updTime), minute(updTime), second(updTime));
                    else Serial.println(": never");
                }
                state.wsensor.updated = true;
            }

            int rc = wsensorStr.indexOf("OK+RC");
            if(rc != -1) {
                unsigned int ch = wsensorStr.substring(rc + 5, rc + 8).toInt();
                if(ch != config.wsensor.channel()) {
                    Serial.println("Changing channel number");
                    if(state.uart2_tx != HC12) {
                        Serial2.setPins(HC12_RX_PIN, HC12_TX_PIN);
                        state.uart2_tx = HC12;
                    }
                    digitalWrite(HC12_SET_PIN, LOW);
                    delay(50);
                    Serial2.printf("AT+C%03d\r\n", config.wsensor.channel());
                    Serial2.flush();
                    vTaskDelay(100);
                    digitalWrite(HC12_SET_PIN, HIGH);
                }
            }
        }
    }
  
    vTaskDelay(1);
    wsensorStr = "";
}