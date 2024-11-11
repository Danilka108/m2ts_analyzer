#ifndef M2TS_ANALYZER_COMMON_H
#define M2TS_ANALYZER_COMMON_H

#include <sys/types.h>

#define SYNC_BYTE 'G'

/*#define CONTROL_FIELDS_OFFSET ((8 + 1 + 1 + 1 + 13 + 2) / 8)*/

#define TS_PACKET_HDR_SIZE 4

#define TS_PACKET_DATA_SIZE 184

#define TS_PACKET_SIZE (TS_PACKET_HDR_SIZE + TS_PACKET_DATA_SIZE)

typedef u_int8_t ts_packet_buf[TS_PACKET_SIZE];

#endif
