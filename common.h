#ifndef M2TS_ANALYZER_COMMON_H
#define M2TS_ANALYZER_COMMON_H

#include <sys/types.h>

#define SYNC_BYTE 'G'

#define NULL_PACKET_PID 0x1FFF

/*#define CONTROL_FIELDS_OFFSET ((8 + 1 + 1 + 1 + 13 + 2) / 8)*/

#define TS_PACKET_HDR_SIZE 4

#define TS_PACKET_DATA_SIZE 184

#define TS_PACKET_SIZE (TS_PACKET_HDR_SIZE + TS_PACKET_DATA_SIZE)

#define PAT_PID 0

#define PAT_TABLE_ID 0

/*#define SECTION_LENGTH_FIELD_MAX_VAL 1021*/
#define PAS_MAX_SIZE 1024

#define PAS_HDR_SIZE 8

#define PAS_CRC_32_SIZE 4

#define PAS_MIN_SIZE (PAS_HDR_SIZE + PAS_CRC_32_SIZE)

#define STUFFING_BYTE 0xFF

#define MASK(n) ((1 << (n)) - 1)
#define GET_BIT(ptr, index) ((uint8_t)((*(uint8_t *)(ptr) >> (index)) & 0x1))
#define GET_BITS(ptr, start_index, end_index)                                  \
  ((uint8_t)((*(uint8_t *)(ptr) >> (start_index)) &                            \
             MASK(end_index - start_index)))
#define GET_SHORT(ptr) (((uint16_t)(*(ptr)) << 8) | (uint16_t)(*((ptr) + 1)))


/*typedef u_int8_t ts_packet_buf[TS_PACKET_SIZE];*/

/*u_int8_t *extract_table_sections(u_int16_t expected_pid, ts_packet_buf buf,*/
/*                                 size_t *len);*/
/**/
/*void end_extracting(u_int8_t **buf, size_t *size, u_int8_t *crc_32);*/
#endif
