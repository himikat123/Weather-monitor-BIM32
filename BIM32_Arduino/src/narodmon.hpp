class Narodmon {
    public: void send();
    private: String _fieldsPrepare(unsigned int fieldNum, String metrics, String mac);
};

/**
 * Send data to norodmon
 */
void Narodmon::send() {
    if(config.cloud.narodmonSend.lat() == "" or config.cloud.narodmonSend.lon() == "") {
        Serial.println("No coordinates");
        return;
    }

    String mac = WiFi.macAddress();
    mac.replace(":", "");
    String buf = "#BIM" + mac;
    buf += "#" + String(config.cloud.narodmonSend.name());
    buf += "#" + String(config.cloud.narodmonSend.lat());
    buf += "#" + String(config.cloud.narodmonSend.lon());
    for(unsigned int i=0; i<12; i++) {
        buf += _fieldsPrepare(i, config.cloud.narodmonSend.metrics(i), mac); 
    }
    buf += "\n##\r\n";
    if(!client.connect("narodmon.ru", 8283, 3000)) Serial.println("failed");
    else {
        //Serial.println(buf);
        client.print(buf);
        Serial.println("Done");
    }
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.println("Successfull. Server returned" + line);
    }
    client.stop();
}

/**
 * Prepare data for the desired field according to the settings
 * @param field number
 * @return a string with field name and data
 */
String Narodmon::_fieldsPrepare(unsigned int fieldNum, String metrics, String mac) {
    float data = agregateSendData.sendingData(
        config.cloud.narodmonSend.sensors(fieldNum),
        config.cloud.narodmonSend.types(fieldNum),
        config.cloud.narodmonSend.wsensors(fieldNum),
        config.cloud.narodmonSend.wtypes(fieldNum)
    );

    if(data != -40400.0) {
        String field = "\n#" + metrics + "_" + mac + "#" + String(data);
        return field;
    }

    return "";
}