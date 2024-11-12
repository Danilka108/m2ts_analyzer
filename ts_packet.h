#ifndef M2TS_ANALYZER_TS_PACKET
#define M2TS_ANALYZER_TS_PACKET

#include "common.h"
#include <cstdint>
#include <optional>

struct ts_packet_buf {
  uint8_t data[TS_PACKET_SIZE];
};

struct ts_packet {
public:
  ts_packet(ts_packet_buf *buf);
  bool is_valid();

  uint8_t sync_byte;
  uint8_t transport_error_indicator;
  uint8_t payload_until_start_indicator;
  uint16_t pid;
  uint8_t adaptation_field_control;
  uint8_t continuity_counter;

  ts_packet_buf *buf;
  uint8_t *data;
};

#endif
