#include "Stream.h"
#include <cstdint>

Stream::Stream(uint8_t type, uint16_t elementary_pid)
    : type(type), elementary_pid(elementary_pid) {}
