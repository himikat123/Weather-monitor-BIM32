class Narodmon {
    public: void send();
    private: String _fieldsPrepare(unsigned int fieldNum, String metrics, String mac);
};

/**
 * Send data to norodmon
 */
void Narodmon::send() {
    if(config.narodmonSend_lat() == "" or config.narodmonSend_lon() == "") {
        Serial.println("No coordinates");
        return;
    }

    String mac = WiFi.macAddress();
    mac.replace(":", "");
    String buf = "#BIM" + mac;
    buf += "#" + config.narodmonSend_name();
    buf += "#" + config.narodmonSend_lat();
    buf += "#" + config.narodmonSend_lon();
    for(unsigned int i=0; i<12; i++) {
        buf += _fieldsPrepare(i, config.narodmonSend_metrics(i), mac); 
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
        config.narodmonSend_sensors(fieldNum),
        config.narodmonSend_types(fieldNum),
        config.narodmonSend_wsensors(fieldNum),
        config.narodmonSend_wtypes(fieldNum)
    );

    if(data != -40400.0) {
        String field = "\n#" + metrics + "_" + mac + "#" + String(data);
        return field;
    }

    return "";
}