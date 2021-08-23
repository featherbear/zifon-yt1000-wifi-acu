#include <functional>

namespace PT_WS {

typedef std::function<void(uint8_t *payload, size_t length)> WSTextEventHandler;
void begin();
void setDataHandler(WSTextEventHandler handler);
void tick();
void loop();
}  // namespace PT_WS