#ifndef M2TS_ANALYZER_STREAM_H
#define M2TS_ANALYZER_STREAM_H

#include <cstdint>
struct Stream {
public:
  uint8_t type;
  uint16_t elementary_pid;

  Stream(uint8_t type, uint16_t elementary_pid);
};

#endif
