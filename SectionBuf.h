#ifndef M2TS_ANALYZER_SECTION_BUF_H
#define M2TS_ANALYZER_SECTION_BUF_H

#include "Program.h"
#include <vector>
class SectionBuf {
public:
  SectionBuf(std::vector<Program> &programs);

  void reset();

  void insert(uint8_t *buf, size_t buf_size);

private:
  static constexpr size_t capacity = 4096;

  std::vector<Program> &programs;

  size_t size;
  bool end_reached;
  uint8_t data[capacity];
};

#endif
