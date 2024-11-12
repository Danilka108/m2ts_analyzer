#ifndef M2TS_ANALYZER_PAT_H
#define M2TS_ANALYZER_PAT_H

#include "common.h"
#include <sys/types.h>

typedef struct pat {
  u_int8_t *data;
  size_t capacity;
  size_t size;
} pat_t;

#define PMS_SIZE 1024

struct pms {
  u_int8_t data[PMS_SIZE];
  size_t size;
  size_t section_length;
};

struct pmt {
  struct pms *sections;
  size_t size;
  size_t capacity;
};

struct streams {
  u_int8_t stream_values[128];
  size_t size;
};

typedef struct program {
  u_int16_t program_number;
  u_int16_t program_map_pid;
  struct streams streams;
  struct pmt pmt;
} program_t;

typedef struct programs {
  program_t *programs;
  size_t size;
  size_t capacity;
  /*struct program *next;*/
} programs_t;

void init_pat(pat_t *pat);

int read_pat(pat_t *pat, ts_packet_buf buf);

int extract_program_numbers(programs_t *list, pat_t *pat);

void free_pat(pat_t *pat);

void free_program_numbers(programs_t *list);

#endif
