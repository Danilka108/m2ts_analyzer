#ifndef M2TS_ANALYZER_PAT_H
#define M2TS_ANALYZER_PAT_H

#include "common.h"
#include <sys/types.h>

typedef struct pat {
  u_int8_t *data;
  size_t capacity;
  size_t size;
} pat_t;

typedef struct pn_node {
  u_int16_t ts_id;
  u_int16_t program_number;
  u_int16_t program_map_pid;
  struct pn_node *next;
} pn_node_t;

typedef struct program_numbers {
  struct pn_node *next;
} program_numbers_t;

void init_pat(pat_t *pat);

int read_pat(pat_t *pat, ts_packet_buf buf);

int extract_program_numbers(program_numbers_t *list, pat_t *pat);

void free_pat(pat_t *pat);

void free_program_numbers(program_numbers_t *list);


#endif
