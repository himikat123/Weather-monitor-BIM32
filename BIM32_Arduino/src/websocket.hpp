class WebSocket {
    private:
        WebSocketsServer* wsServer;
        uint8_t _maxClients;
        bool _active[6];
        //unsigned long _lastPing[6];
        //unsigned long _lastPong[6];
        //const unsigned long _pingInterval = 30000;
        //const unsigned long _pongTimeout = 10000;

        void _handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
        //void _handlePingPong();

    public:
        WebSocket(uint8_t port, uint8_t maxClients);
        void begin();
        void loop();
        void sendText(String &msg);
        void sendJson(String &json);
        bool hasClient();
};

void WebSocket::_handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WS %u] Disconnected\n", num);
            //_lastPing[num] = 0;
            //_lastPong[num] = 0;
            _active[num] = false;
            break;
        case WStype_CONNECTED: {
            IPAddress ip = wsServer->remoteIP(num);
            Serial.printf("[WS %u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
            //wsServer->sendTXT(num, "Hello from ESP32 WS");
            //_lastPing[num] = millis();
            //_lastPong[num] = millis();
            _active[num] = true;
            break;
        }
        case WStype_TEXT:
            Serial.printf("[WS %u] Received: %s\n", num, payload);
            wsServer->sendTXT(num, String("{\"echo\": \"") + (char*)payload) + String("\"}");
            break;
        case WStype_PONG:
            //_lastPong[num] = millis();
            break;
        default:
            break;
    }
}

//void WebSocket::_handlePingPong() {
//    unsigned long now = millis();
//    for(uint8_t i=0; i<_maxClients; i++) {
//        if(_active[i]) {
//            if(now - _lastPing[i] > _pingInterval) {
//                wsServer->sendPing(i);
//                _lastPing[i] = now;
//            }
//            if(now - _lastPong[i] > _pongTimeout) {
//                Serial.printf("[WS %u] Pong timeout → disconnecting\n", i);
//                wsServer->disconnect(i);
//                _lastPing[i] = 0;
//                _lastPong[i] = 0;
//                _active[i] = false;
//            }
//        }
//    }
//}

WebSocket::WebSocket(uint8_t port, uint8_t maxClients) {
    wsServer = new WebSocketsServer(port);
    this->_maxClients = maxClients;
    for(uint8_t i=0; i<maxClients; i++) {
        //_lastPing[i] = 0;
        //_lastPong[i] = 0;
        _active[i] = false;
    }
}

void WebSocket::begin() {
    //wsServer->enableHeartbeat(15000, 3000, 2); // LOGS delete this
    wsServer->begin();
    //Serial.println("WS listening on port 88...");
    wsServer->onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length){
        this->_handleEvent(num, type, payload, length);
    });
}

void WebSocket::loop() {
    wsServer->loop();
    //_handlePingPong();
}

void WebSocket::sendText(String &msg) {
    for(uint8_t i=0; i<_maxClients; i++) {
        if(_active[i]) wsServer->sendTXT(i, msg);
    }
}

void WebSocket::sendJson(String &json) {
    sendText(json);
}

bool WebSocket::hasClient() {
    for(uint8_t i=0; i<_maxClients; i++)
        if(_active[i]) return true;
    return false;
}