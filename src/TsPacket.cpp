#include "TsPacket.h"
#include "common.h"

uint8_t *calc_data_ptr(TsPacketBuf &buf, uint8_t afc);

TsPacket::TsPacket(TsPacketBuf &buf)
    : buf(buf), sync_byte(*buf.data),
      transport_error_indicator(GET_BIT(buf.data + 1, 7)),
      payload_until_start_indicator(GET_BIT(buf.data + 1, 6)),
      pid(GET_SHORT(buf.data + 1) & MASK(13)),
      adaptation_field_control(GET_BITS(buf.data + 3, 4, 6)),
      continuity_counter(GET_BITS(buf.data + 3, 0, 4)),
      data(calc_data_ptr(buf, adaptation_field_control)),
      end(buf.data + TS_PACKET_SIZE), data_len(end - data) {}

bool TsPacket::is_valid() {
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

bool TsPacket::is_continuous(int8_t last_cc) {
  bool is_discontinuity = (adaptation_field_control & 2) && buf.data[4] > 0 &&
                          ((buf.data[5] >> 7) & 1);
  uint8_t expected_cc =
      adaptation_field_control & 1 ? ((last_cc + 1) & 0x1FFF) : last_cc;

  return pid == NULL_PACKET_PID || is_discontinuity || last_cc < 0 ||
         expected_cc == continuity_counter;
}

uint8_t *calc_data_ptr(TsPacketBuf &buf, uint8_t afc) {
  uint8_t *data = buf.data + TS_PACKET_HDR_SIZE;

  if (afc == 0b10 || afc == 0b11) {
    data += *data;
  }

  return data;
}
