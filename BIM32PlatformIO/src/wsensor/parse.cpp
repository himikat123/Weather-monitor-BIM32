// #include <Arduino.h>
#include <ArduinoJson.h>
#include <TimeLib.h>

#include "./wirelessSensor.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "../globals.hpp"
#include "../pinout.hpp"

void WirelessSensor::parse() {
    if(wirelessSensorQueue == NULL) return;

    String wsensorStr = "";

    if(xQueueReceive(wirelessSensorQueue, &wsensorStr, 0) == pdPASS) {
        int startJson = wsensorStr.indexOf("{");
        int endJson = wsensorStr.indexOf("}");

        while(startJson != -1 && endJson != -1) {
            JsonDocument root;
            String js = wsensorStr.substring(startJson, endJson + 1);
            wsensorStr = wsensorStr.substring(endJson + 1);

            DeserializationError error = deserializeJson(root, js);
            if(!error) {
                int number = root["num"];
                if(number >= 0 && number < WSENSORS) {
                    state.wsensor.time[number] = now();

                    state.wsensor.temp.data[0][number] = root["t"] | 40400.0;
                    strlcpy(state.wsensor.temp.name[0][number], root["s"] | "", sizeof(state.wsensor.temp.name[0][number]));

                    for(int i=1; i<5; i++) {
                        state.wsensor.temp.data[i][number] = root["ds"][i - 1] | 40400.0;
                        strlcpy(state.wsensor.temp.name[i][number], "DS18B20", sizeof(state.wsensor.temp.name[i][number]));
                    }

                    state.wsensor.hum.data[number] = root["h"] | 40400.0;
                    strlcpy(state.wsensor.hum.name[number], root["s"] | "", sizeof(state.wsensor.hum.name[number]));

                    state.wsensor.pres.data[number] = root["p"] | 40400.0;
                    strlcpy(state.wsensor.pres.name[number], root["s"] | "", sizeof(state.wsensor.pres.name[number]));

                    state.wsensor.wind.speed.data[number] = root["wind"][0] | -1.0;
                    strlcpy(state.wsensor.wind.speed.name[number], "RS485", sizeof(state.wsensor.wind.speed.name[number]));
                    state.wsensor.wind.dir.data[number] = root["wind"][1] | -1.0;
                    strlcpy(state.wsensor.wind.dir.name[number], "RS485", sizeof(state.wsensor.wind.dir.name[number]));

                    state.wsensor.light.data[number] = root["l"] | -1.0;
                    strlcpy(state.wsensor.light.name[number], root["a"] | "", sizeof(state.wsensor.light.name[number]));

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

                    state.wsensor.co2.data[number] = root["s8"] | -1.0;
                    strlcpy(state.wsensor.co2.name[number], "Senseair S8", sizeof(state.wsensor.co2.name[number]));

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
                }
                state.wsensor.updated = true;
            }

            int rc = js.indexOf("OK+RC");
            if(rc != -1) {
                unsigned int ch = js.substring(rc + 5, rc + 8).toInt();
                if(ch != config.wsensor.channel()) {
                    digitalWrite(HC12_SET_PIN, LOW);
                    vTaskDelay(pdMS_TO_TICKS(50));
                    Serial2.printf("AT+C%03d\r\n", config.wsensor.channel());
                    Serial2.flush();
                    vTaskDelay(pdMS_TO_TICKS(100));
                    digitalWrite(HC12_SET_PIN, HIGH);
                }
            }

            startJson = wsensorStr.indexOf("{");
            endJson = wsensorStr.indexOf("}");
        }
    }
}