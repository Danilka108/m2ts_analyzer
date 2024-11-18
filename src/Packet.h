#ifndef M2TS_ANALYZER_PACKET_H
#define M2TS_ANALYZER_PACKET_H

#include "common.h"

#include <cstddef>
#include <cstdint>

class Packet {
public:
  Packet() = delete;

  char get_sync_byte() const {
    return data[0];
  }

  uint8_t get_TEI() const {
    return GET_BIT(data + 1, 7);
  }

  uint8_t get_PUSI() const {
    return GET_BIT(data + 1, 6);
  }

  uint16_t get_PID() const {
    return GET_SHORT(data + 1) & MASK(13);
  }

  uint8_t get_AFC() const {
    return GET_BITS(data + 3, 4, 6);
  }

  uint8_t get_CC() const {
    return GET_BITS(data + 3, 0, 4);
  }

  const uint8_t *get_payload() const {
    return data + HDR_SIZE;
  }

  const uint8_t *get_CRC() const {
    return data + SIZE - 4;
  }

  static constexpr size_t SIZE = 188;

  static constexpr size_t HDR_SIZE = 4;

  static constexpr char SYNC_BYTE = 0x47;

private:
  uint8_t data[SIZE];
};

#endif
