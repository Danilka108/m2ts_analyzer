#include "ts_packet.h"
#include "common.h"
#include <cstdint>

#define MASK(n) ((1 << (n)) - 1)
#define GET_BIT(ptr, index) ((uint8_t)((*(uint8_t *)(ptr) >> (index)) & 0x1))
#define GET_BITS(ptr, start_index, end_index)                                  \
  ((uint8_t)((*(uint8_t *)(ptr) >> (start_index)) &                            \
             MASK(end_index - start_index)))
#define GET_SHORT(ptr) (((uint16_t)(*(ptr)) << 8) | (uint16_t)(*(ptr + 1)))

ts_packet::ts_packet(ts_packet_buf *buf)
    : sync_byte(*buf->data),
      transport_error_indicator(GET_BIT(buf->data + 1, 7)),
      payload_until_start_indicator(GET_BIT(buf->data + 1, 6)),
      pid(GET_SHORT(buf->data + 1) & MASK(13)),
      adaptation_field_control(GET_BITS(buf->data + 3, 4, 6)), buf(buf),
      continuity_counter(GET_BITS(buf->data + 3, 0, 4)),
      data(buf->data + TS_PACKET_HDR_SIZE) {
  if (adaptation_field_control == 0b10 || adaptation_field_control == 0b11) {
    data += *data;
  }
}

bool ts_packet::is_valid() {
  if (sync_byte != SYNC_BYTE) {
    return false;
  }

  if (transport_error_indicator == 1) {
    return false;
  }

  if (adaptation_field_control == 0b00) {
    return false;
  }

  return true;
}
