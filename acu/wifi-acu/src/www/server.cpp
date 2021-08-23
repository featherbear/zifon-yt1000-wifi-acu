
#include "server.hpp"

#include <Hash.h>
#include <LittleFS.h>

#include "www.hpp"

namespace PT_WWW {

std::unique_ptr<WebServer> server = NULL;
String serverMode = "";

static String getContentType(String filename) {
    if (filename.endsWith(".html"))
        return FPSTR(CONTENT_TYPES::HTML);
    else if (filename.endsWith(".css"))
        return FPSTR(CONTENT_TYPES::CSS);
    else if (filename.endsWith(".js"))
        return FPSTR(CONTENT_TYPES::JS);
    else if (filename.endsWith(".ico"))
        return FPSTR(CONTENT_TYPES::ICO);

    // Unknown?
    return FPSTR(CONTENT_TYPES::PLAIN);
}

void setMode(String mode) {
    serverMode = mode;
}

void init() {
    server = std::unique_ptr<WebServer>(new WebServer(80));

    server->on(FPSTR("/mode"), []() {
        if (PT_WWW::server->method() != HTTP_GET) return PT_WWW::server->close();
        PT_WWW::server->send(200, FPSTR(CONTENT_TYPES::PLAIN), serverMode);
    });
}

void begin() {
    server->serveStatic("/", LittleFS, "/www/");

    server->onNotFound([]() {
        String path = server->uri();

        if (path.endsWith("/")) path += "index.html";

        if (!LittleFS.exists(path)) {
            Serial.println("\tFile Not Found");
            server->send(404, FPSTR(CONTENT_TYPES::PLAIN), "404: Not Found");
            return;
        }

        File file = LittleFS.open(path, "r");
        server->streamFile(file, getContentType(path));
        file.close();
    });
    server->begin();
}

void tick() {
    server->handleClient();
}
void loop() {
    while (true) tick();
}
}  // namespace PT_WWW