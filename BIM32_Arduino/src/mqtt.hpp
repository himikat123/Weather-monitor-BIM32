#include <PubSubClient.h> // v2.8.0 https://github.com/knolleary/pubsubclient
PubSubClient mqttClient(client);

class MQTT {
    public:
        void init();
        bool loop();
        void send();
    private:
        void _reconnect(); 
        bool _sendSensorData(unsigned int sensNum);
};

/**
 * Initialize MQTT client
 */
void MQTT::init() {
    mqttClient.setServer(config.mqttSend_broker(), config.mqttSend_port());
}

/**
 * MQTT client loop
 */
bool MQTT::loop() {
    if(!mqttClient.connected()) {
        _reconnect();
    }
    mqttClient.loop();
    return mqttClient.connected();
}

/**
 * MQTT client reconnect
 */
void MQTT::_reconnect() {
    Serial.println(SEPARATOR);
    Serial.print("Attempting MQTT connection...");
    if(mqttClient.connect(
        ("BIM32_" + WiFi.macAddress()).c_str(), 
        config.mqttSend_user(), 
        config.mqttSend_pass())
    ) Serial.println("connected");
    else {
        Serial.print("failed, rc="); 
        Serial.println(mqttClient.state());
    }
}

/**
 * Send data via MQTT
 */
void MQTT::send() {
    bool result = false;
    for(unsigned int i=0; i<12; i++) {
        result |= _sendSensorData(i); 
    }
    Serial.println(result ? "sent" : "error");
}

/**
 * Sending data from one sensor
 * @param sensNum sensor number
 */
bool MQTT::_sendSensorData(unsigned int sensNum) {
    float data = agregateSendData.sendingData(
        config.mqttSend_sensors(sensNum),
        config.mqttSend_types(sensNum),
        config.mqttSend_wsensors(sensNum),
        config.mqttSend_wtypes(sensNum)
    );

    if(data != -40400.0) {
        char cData[12] = "";
        snprintf(cData, 12, "%.2f", data);
        return mqttClient.publish(config.mqttSend_topics(sensNum), cData);
    }
    return false;
}