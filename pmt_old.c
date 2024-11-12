#include "pmt.h"
#include "common.h"
#include "pat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define CAPACITY_RESIZE_FACTOR 2

void init_pmt(pmt_t *pmt) {
  if (pmt == NULL)
    return;

  pmt->data = NULL;
  pmt->size = 0;
  pmt->capacity = 0;
}

void free_pmt(pmt_t *pmt) {
  if (pmt == NULL)
    return;
  free(pmt->data);
}

int extract_program_info(programs_t *programs, u_int8_t **buf, size_t *size);

void add_stream(programs_t *programs, u_int16_t program_number,
                u_int8_t stream_type);

int read_pmt(programs_t *programs, ts_packet_buf buf) {

  /*
  size_t len = 0;
  u_int8_t *data = NULL;

  for (program_t *node = programs->next; node; node = node->next) {
    data = extract_table_sections(node->program_map_pid, buf, &len);
    if (data == NULL) {
      return -1;
    }
  }

  if (pmt->size + len > pmt->capacity) {
    u_int8_t *new_data = malloc(pmt->capacity * CAPACITY_RESIZE_FACTOR);

    if (new_data == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }

    memcpy(new_data, pmt->data, pmt->size);
    free(pmt->data);

    pmt->data = new_data;
    pmt->capacity *= CAPACITY_RESIZE_FACTOR;
  }

  memcpy(pmt->data + pmt->size, data, len);
  pmt->size += len;

  return 0;
  */
}

u_int8_t *extract_pms(programs_t *programs, ts_packet_buf buf, size_t *len) {
  size_t program_i = 0;
  int packet_found = 0;

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

  for (program_i = 0, packet_found = 0; program_i < programs->size; program_i++) {
    if (programs->programs[program_i].program_map_pid == pid) {
      packet_found = 1;
      break;
    }
  }

  if (!packet_found) {
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

int extract_programs_info(pmt_t *pmt, programs_t *programs) {
  u_int8_t *buf = pmt->data;
  size_t size = pmt->size;
  int result = 0;

  while (result != 1) {
    result = extract_program_info(programs, &buf, &size);
    if (result == -1) {
      fprintf(stderr, "failed to extract pmt info\n");
      return -1;
    }
  }

  return 0;
}

int extract_program_info(programs_t *programs, u_int8_t **buf, size_t *size) {
  if (*size < 96) {
    return 1;
  }

  u_int8_t *hdr_start = *buf;

  u_int8_t table_id = *hdr_start;
  u_int8_t section_syntax_indicator = (*(hdr_start + 1) >> 7) & 0b1;
  u_int8_t zero_field = (*(hdr_start + 1) >> 6) & 0b1;
  u_int16_t section_length = ((u_int16_t)(*(hdr_start + 1) & 0b1111) << 8) |
                             (u_int16_t)(*(hdr_start + 2));
  u_int16_t program_number =
      (u_int16_t)(*(hdr_start + 3) << 8) | (u_int16_t)(*(hdr_start + 4));
  u_int8_t current_next_indicator = *(hdr_start + 5) & 0b1;
  u_int8_t section_number = *(hdr_start + 6);
  u_int8_t last_section_number = *(hdr_start + 7);
  u_int16_t program_info_length =
      ((u_int16_t)(*(hdr_start + 9) & 0b1111) << 8) |
      (u_int16_t)(*(hdr_start + 10));

  u_int8_t *stream_info_start = hdr_start + 12 + program_info_length;
  u_int8_t *crc_32 = hdr_start + 3 + section_length - 4;

  u_int8_t stream_type = 0;
  u_int16_t es_info_length = 0;

  if (table_id != 0x02) {
    end_extracting(buf, size, crc_32);
    return 0;
  }

  if (section_syntax_indicator != 1) {
    end_extracting(buf, size, crc_32);
    return 0;
  }

  if (current_next_indicator != 1) {
    end_extracting(buf, size, crc_32);
    return 0;
  }

  if (*size < (crc_32 + 4) - *buf) {
    return 1;
  }

  while (1) {
    stream_type = *stream_info_start;
    es_info_length = ((u_int16_t)(*(stream_info_start + 2) & 0b1111) << 8) |
                     (u_int16_t)(*(stream_info_start + 3));
    stream_info_start += 5 + es_info_length;

    add_stream(programs, program_number, stream_type);
  }

  end_extracting(buf, size, crc_32);
}

void add_stream(programs_t *programs, u_int16_t program_number,
                u_int8_t stream_type) {
  for (program_t *node = programs->next; node; node = node->next) {
    if (node->program_number != program_number) {
      continue;
    }

    if (node->streams_size + 1 >=
        sizeof(node->streams) / sizeof(node->streams[0])) {
      continue;
    }

    node->streams[node->streams_size] = stream_type;
    node->streams_size += 1;
    return;
  }
}
