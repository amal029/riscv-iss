#include <cstddef>
#include <cstdint>

struct Fetch {
  static uint32_t fetch(uint8_t *MEM, size_t PC) {
    return *((uint32_t *)(MEM + PC));
  }
};
