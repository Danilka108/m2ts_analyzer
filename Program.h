#ifndef M2TS_ANALYZER_PROGRAM_H
#define M2TS_ANALYZER_PROGRAM_H

#include <cstdint>

struct Program {
public:
  uint16_t id;
  uint16_t map_pid;

  Program(uint16_t id, uint16_t map_pid);
};

#endif
