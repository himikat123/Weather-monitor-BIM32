#pragma once

class WebSocket {
    private:
        WebSocketsServer* wsServer;
        uint8_t _maxClients = 1;
        bool _active[6];
        String _lastReceived = "";
        bool _hasNewMessage = false;

        void _handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

    public:
        WebSocket(uint8_t port, uint8_t maxClients);
        void begin();
        void loop();
        void sendText(String &msg);
        void sendJson(String &json);
        bool hasClient();
        bool available();
        String read();
};