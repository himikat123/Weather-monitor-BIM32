class WebSocket {
    private:
        WebSocketsServer* wsServer;
        uint8_t _maxClients = 1;
        bool _active[6];
        void _handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

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
            _active[num] = false;
            break;
        case WStype_CONNECTED: {
            IPAddress ip = wsServer->remoteIP(num);
            Serial.printf("[WS %u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
            _active[num] = true;
            break;
        }
        case WStype_TEXT:
            Serial.printf("[WS %u] Received: %s\n", num, payload);
            wsServer->sendTXT(num, String("{\"echo\": \"") + (char*)payload) + String("\"}");
            break;
        default: break;
    }
}

WebSocket::WebSocket(uint8_t port, uint8_t maxClients) {
    wsServer = new WebSocketsServer(port);
    this->_maxClients = maxClients;
    for(uint8_t i=0; i<maxClients; i++) {
        _active[i] = false;
    }
}

void WebSocket::begin() {
    wsServer->begin();
    wsServer->onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length){
        this->_handleEvent(num, type, payload, length);
    });
}

void WebSocket::loop() {
    wsServer->loop();
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