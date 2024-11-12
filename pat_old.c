#include "pat.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <malloc.h>*/
#include <sys/types.h>

#define PAS_LEN (8 + 1 + 1 + 2 + 12 + 1021)
#define CAPACITY_RESIZE_FACTOR 2

/*u_int8_t *extract_pat_sections(ts_packet_buf buf, size_t *len);*/

int extract_pas_program_numbers(programs_t *list, u_int8_t **buf, size_t *size);

void add_program_number(programs_t *list, u_int16_t ts_id,
                        u_int16_t program_number, u_int16_t program_map_pid);

void init_pat(pat_t *pat) {
  pat->data = malloc(TS_PACKET_SIZE);
  if (pat->data == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  pat->capacity = TS_PACKET_SIZE;
  pat->size = 0;
}

int read_pat(pat_t *pat, ts_packet_buf buf) {
  size_t len = 0;
  u_int8_t *data = NULL;

  data = extract_table_sections(0, buf, &len);
  /*data = extract_pat_sections(buf, &len);*/
  if (data == NULL) {
    return -1;
  }

  if (pat->size + len > pat->capacity) {
    u_int8_t *new_data = malloc(pat->capacity * CAPACITY_RESIZE_FACTOR);

    if (new_data == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }

    memcpy(new_data, pat->data, pat->size);
    free(pat->data);

    pat->data = new_data;
    pat->capacity *= CAPACITY_RESIZE_FACTOR;
  }

  memcpy(pat->data + pat->size, data, len);
  pat->size += len;

  return 0;
}

void free_pat(pat_t *pat) { free(pat->data); }

/*
u_int8_t *extract_pat_sections(ts_packet_buf buf, size_t *len) {
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

  if (pid != 0) {
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
    // *len = TS_PACKET_SIZE - (data_start - buf) + 1;
    *len = TS_PACKET_SIZE - (data_start - buf);
  }

  return data_start;
}
*/

int extract_program_numbers(programs_t *list, pat_t *pat) {
  u_int8_t *buf = pat->data;
  size_t size = pat->size;
  int result = 0;

  while (result != 1) {
    result = extract_pas_program_numbers(list, &buf, &size);
    /*if (result == -1) {*/
    /*  fprintf(stderr, "failed to extract pas program numbers\n");*/
    /*  return -1;*/
    /*}*/
  }

  return 0;
}

int extract_pas_program_numbers(programs_t *list, u_int8_t **buf,
                                size_t *size) {
  if (*size < 12) {
    return 1;
  }

  u_int8_t *hdr_start = *buf + 1 + **buf;

  u_int8_t table_id = *hdr_start;
  u_int8_t section_syntax_indicator = (*(hdr_start + 1) >> 7) & 0b1;
  u_int8_t zero_field = (*(hdr_start + 1) >> 6) & 0b1;
  u_int16_t section_length = ((u_int16_t)(*(hdr_start + 1) & 0b1111) << 8) |
                             (u_int16_t)(*(hdr_start + 2));
  u_int16_t ts_id = *(u_int16_t *)(hdr_start + 3);
  u_int8_t current_next_indicator = *(hdr_start + 5) & 0b1;
  u_int8_t section_number = *(hdr_start + 6);
  u_int8_t last_section_number = *(hdr_start + 7);

  u_int8_t *program_number_start = hdr_start + 8;
  u_int8_t *crc_32 = hdr_start + 3 + section_length - 4;

  u_int16_t program_number = 0;
  u_int16_t program_map_pid;

  if (table_id != 0) {
    end_extracting(buf, size, crc_32);
    return 0;
  }

  if (section_syntax_indicator != 1) {
    end_extracting(buf, size, crc_32);
    return 0;
  }

  if (zero_field != 0) {
    end_extracting(buf, size, crc_32);
    return 0;
  }

  if (section_length > 1021) {
    end_extracting(buf, size, crc_32);
    return 0;
  }

  if (current_next_indicator == 0) {
    end_extracting(buf, size, crc_32);
    return 0;
  }

  if (*size < (crc_32 + 4) - *buf) {
    return 1;
  }

  for (; program_number_start < crc_32; program_number_start += 4) {
    program_number = (((u_int16_t)*program_number_start) << 8) |
                     (u_int16_t) * (program_number_start + 1);
    if (program_number == 0) {
      continue;
    }

    program_map_pid =
        (((u_int16_t)(*(program_number_start + 2)) & 0b11111) << 8) |
        ((u_int16_t) * (program_number_start + 3));

    add_program_number(list, ts_id, program_number, program_map_pid);
  }

  *size -= (crc_32 + 4) - *buf;
  *buf = crc_32 + 4;

  while (**buf == 0xFF) {
    *buf += 1;
  }

  return 0;
}

void add_program_number(programs_t *list, u_int16_t ts_id,
                        u_int16_t program_number, u_int16_t program_map_pid) {
  for (program_t *node = list->next; node != NULL; node = node->next) {
    if (node->program_number == program_number) {
      return;
    }
  }

  program_t *new_node = malloc(sizeof(program_t));
  if (new_node == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  new_node->next = list->next;
  list->next = new_node;
  new_node->program_number = program_number;
  new_node->program_map_pid = program_map_pid;
  new_node->streams_size = 0;
  new_node->pmt_buf.data = malloc(TS_PACKET_SIZE);
  if (new_node->pmt_buf.data == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  new_node->pmt_buf.size = 0;
  new_node->pmt_buf.capacity = TS_PACKET_SIZE;
}

void free_program_numbers(programs_t *list) {
  program_t *node = list->next;
  program_t *node_to_free = NULL;

  list->next = NULL;

  while (node != NULL) {
    node_to_free = node;
    node = node->next;

    free(node_to_free);
  }
}
