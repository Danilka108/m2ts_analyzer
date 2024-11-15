#ifndef M2TS_ANALYZER_PROGRAM_H
#define M2TS_ANALYZER_PROGRAM_H

#include "Stream.h"
#include <cstdint>
#include <vector>

struct Program {
public:
  Program(uint16_t id, uint16_t map_pid);

  uint16_t id;
  uint16_t map_pid;
  bool pmt_found;
  std::vector<Stream> streams;
};

#endif
