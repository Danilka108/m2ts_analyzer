#include "common.h"
#include <stdlib.h>
#include <sys/types.h>

u_int8_t *extract_table_sections(u_int16_t expected_pid, ts_packet_buf buf,
                                 size_t *len) {
  u_int8_t sync_byte = *buf;
  u_int8_t transport_error_indicator = (*(u_int8_t *)(buf + 1) >> 7) & 0b1;
  u_int8_t payload_until_start_indicator = (*(u_int8_t *)(buf + 1) >> 6) & 0b1;
  u_int16_t pid =
      (((u_int16_t)(*(buf + 1)) & 0b11111) << 8) | (u_int16_t)(*(buf + 2));
  u_int8_t adaptation_field_control = (*(u_int8_t *)(buf + 3) >> 4) & 0b11;
  u_int8_t *data_start = buf + TS_PACKET_HDR_SIZE;

  if (sync_byte != 'G') {
    return NULL;
  }

  if (transport_error_indicator == 1) {
    return NULL;
  }

  if (pid != expected_pid) {
    return NULL;
  }

  if (adaptation_field_control == 0b00 || adaptation_field_control == 0b10) {
    return NULL;
  }

  if (adaptation_field_control == 0b11) {
    data_start += *data_start;
  }

  if (payload_until_start_indicator == 1) {
    data_start += *data_start;
  }

  if (len != NULL) {
    *len = TS_PACKET_SIZE - (data_start - buf);
  }

  return data_start;
}

void end_extracting(u_int8_t **buf, size_t *size, u_int8_t *crc_32) {
  *size -= (crc_32 + 4) - *buf;
  *buf = crc_32 + 4;

  while (**buf == 0xFF) {
    *buf += 1;
  }
}
