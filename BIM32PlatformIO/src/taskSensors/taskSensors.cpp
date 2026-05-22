#include <Arduino.h>

#include "../globals.hpp"
#include "../pinout.hpp"
#include "../config/config.hpp"
#include "../state/state.hpp"
#include "./taskSensors.hpp"

TaskSensors::TaskSensors() {}

TaskSensors::~TaskSensors() {
    if(_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
}

bool TaskSensors::start(const char* name, uint32_t stackSize, UBaseType_t priority, BaseType_t coreId) {
    //int type = config.display.type(_dispNum);
    //int model = config.display.model(_dispNum);

    BaseType_t result = xTaskCreatePinnedToCore(TaskSensors::_taskWrapper, name, stackSize, this, priority, &_taskHandle, coreId);
    return (result == pdPASS);
}

void TaskSensors::_taskWrapper(void* pvParameters) {
    TaskSensors* instance = static_cast<TaskSensors*>(pvParameters);
    instance->_run(); 
}

void TaskSensors::_run() {
    attachInterruptArg(DISPLAY1_BUTTON_PIN, isr_display1, this, FALLING);
    attachInterruptArg(DISPLAY2_BUTTON_PIN, isr_display2, this, FALLING);
    attachInterruptArg(ALARM_BUTTON_PIN, isr_alarm, this, FALLING);
    attachInterruptArg(MP3_BUSY_PIN, isr_mp3, this, FALLING);

    sensors.init();

    // HC12 wireless module channel number request
    digitalWrite(HC12_SET_PIN, LOW);
    delay(50);
    Serial2.println("AT+RC");
    Serial2.flush();
    delay(100);
    digitalWrite(HC12_SET_PIN, HIGH);

    sound.init();
    mqtt.init();

    network.scanNetworks();

    while(1) {
        // Enter access point mode if "Settings" button is pressed
        if(digitalRead(SETTINGS_BUTTON_PIN) == 0) network.runAccessPoint();

        /**
         * Wired sensors update
         * & comfort calculate. 
         * every 5 seconds
         */
        if(millis() - sensors_update > 5000) {
            sensors_update = millis();
            if(sensorsSemaphore != NULL) {
                if(xSemaphoreTake(sensorsSemaphore, (TickType_t)100) == pdTRUE) {
                    sensors.read();
                    xSemaphoreGive(sensorsSemaphore);
                }
            }
            comfort.calculate();
            comfort.devicesControl();

            if(state.apMode) strlcpy(state.network.ssid, config.accessPoint.ssid(), sizeof(state.network.ssid));
            else WiFi.SSID().toCharArray(state.network.ssid, sizeof(state.network.ssid));
            state.network.ch = WiFi.channel();
            state.network.sig = WiFi.RSSI();
            uint8_t mac[6];
            WiFi.macAddress(mac);
            snprintf(state.network.mac, sizeof(state.network.mac), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            IPAddress ip = WiFi.localIP();
            IPAddress mask = WiFi.subnetMask();
            IPAddress gw = WiFi.gatewayIP();
            IPAddress dns1 = WiFi.dnsIP();
            IPAddress dns2 = WiFi.dnsIP();
            snprintf(state.network.ip, sizeof(state.network.ip), "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
            snprintf(state.network.mask, sizeof(state.network.mask), "%u.%u.%u.%u", mask[0], mask[1], mask[2], mask[3]);
            snprintf(state.network.gw, sizeof(state.network.gw), "%u.%u.%u.%u", gw[0], gw[1], gw[2], gw[3]);
            snprintf(state.network.dns1, sizeof(state.network.dns1), "%u.%u.%u.%u", dns1[0], dns1[1], dns1[2], dns1[3]);
            snprintf(state.network.dns2, sizeof(state.network.dns2), "%u.%u.%u.%u", dns2[0], dns2[1], dns2[2], dns2[3]);
            state.network.updated = true;
        }

        wsensor.receive(); /* Receive from wireless sensor */

        /**
         * Network connection if not connected and if not Access point mode
         */
        if(WiFi.localIP().toString() == "0.0.0.0" or !network.isConnected()) {
            state.net_connected = false;
            network.connect();
            vTaskDelay(1000);
        }
        else state.net_connected = true;
        
        if(!state.apMode) {
            /**
             * Time synchronization with NTP server
             */
            if(config.clock.ntpPeriod() > 0) {
                if((millis() - ntp_update) > config.clock.ntpPeriod() * 60000 or !state.clockSynchronized) {
                    ntp_update = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("NTP synchronization... ");
                    if(network.isConnected()) timeNTP.syncNTP();
                    else {
                        state.clockSynchronized = false;
                        Serial.println("No internet connection");
                    }
                    vTaskDelay(100);
                }
            }

            /**
             * Receive data from thingspeak
             */
            if(config.cloud.thingspeakReceive.period() > 0) {
                if((millis() - thingspeakReceive) > (config.cloud.thingspeakReceive.period() * 60000) or thingspeakReceive == 0) {
                    thingspeakReceive = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("Receive data from thingspeak.com... ");
                    if(network.isConnected()) thingspeak.receive();
                    else {
                        thingspeakReceive = 0;
                        Serial.println("No internet connection");
                    }
                }
            }

            /**
             * Send data to thingspeak
             */
            if(config.cloud.thingspeakSend.period() > 0) {
                if((millis() - thingspeakSend) > (config.cloud.thingspeakSend.period() * 60000) or thingspeakSend == 0) {
                    thingspeakSend = millis();
                    Serial.println(SEPARATOR);
                    Serial.println("Send data to thingspeak.com... ");
                    if(network.isConnected()) thingspeak.send();
                    else {
                        thingspeakSend = 0;
                        Serial.println("No internet connection");
                    }
                }
            }


            _sendNarodmon();
            _sendMqtt();
            _updateWeather();
            _updateHistoryRepository();
        }

        comfort.soundNotify();
        sound.hourlySignal(); /* Hourly signal */
        sound.alarm(); /* Alarm */
        network.setNeedToPing();

        vTaskDelay(50);
    }
}