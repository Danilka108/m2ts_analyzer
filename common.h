#ifndef M2TS_ANALYZER_COMMON_H
#define M2TS_ANALYZER_COMMON_H

#include <sys/types.h>

#define SYNC_BYTE 'G'

/*#define CONTROL_FIELDS_OFFSET ((8 + 1 + 1 + 1 + 13 + 2) / 8)*/

#define TS_PACKET_HDR_SIZE 4

#define TS_PACKET_DATA_SIZE 184

#define TS_PACKET_SIZE (TS_PACKET_HDR_SIZE + TS_PACKET_DATA_SIZE)

/*typedef u_int8_t ts_packet_buf[TS_PACKET_SIZE];*/

/*u_int8_t *extract_table_sections(u_int16_t expected_pid, ts_packet_buf buf,*/
/*                                 size_t *len);*/
/**/
/*void end_extracting(u_int8_t **buf, size_t *size, u_int8_t *crc_32);*/
#endif
