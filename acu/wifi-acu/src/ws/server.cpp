#include "server.hpp"

#include <WebSocketsServer.h>

// https://shawnhymel.com/1882/how-to-create-a-web-server-with-websockets-using-an-esp32-in-arduino/

namespace PT_WS {
WebSocketsServer server(1337);

WSTextEventHandler eventHandler;

void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t *payload, size_t length) {
    // Figure out the type of WebSocket event
    switch (type) {
        // case WStype_DISCONNECTED:
        //     // Serial.printf("[%u] Disconnected!\n", client_num);
        //     break;

        // case WStype_CONNECTED: {
        //     // IPAddress ip = webSocket.remoteIP(client_num);
        //     // Serial.printf("[%u] Connection from ", client_num);
        //     // Serial.println(ip.toString());
        // }
        // break;

        // Handle text messages from client
        case WStype_TEXT:
        case WStype_BIN:
            // Serial.printf("[%u] Received data: %s\n", client_num, payload);
            if (eventHandler) eventHandler(payload, length);
            break;

        default:
            break;
    }
}

void setDataHandler(WSTextEventHandler handler) {
    eventHandler = handler;
}

void begin() {
    server.onEvent(onWebSocketEvent);
    server.begin();
}

void tick() {
    server.loop();
}
void loop() {
    while (true) tick();
}

}  // namespace PT_WS