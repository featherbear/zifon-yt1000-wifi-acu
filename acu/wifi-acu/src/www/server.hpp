
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#define WebServer ESP8266WebServer
#else
#include <WebServer.h>
#endif

namespace PT_WWW {

extern std::unique_ptr<WebServer> server;
void init();
void begin();
void tick();
void loop();
void setMode(String mode);
}  // namespace PT_WWW