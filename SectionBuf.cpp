#include "SectionBuf.h"
#include "PatSection.h"
#include "common.h"
#include <cstring>

SectionBuf::SectionBuf(std::vector<Program> &programs) : programs(programs) {}

void SectionBuf::reset() {
  size = 0;
  end_reached = false;
}

void SectionBuf::insert(uint8_t *buf, size_t buf_size) {
  if (end_reached) {
    return;
  }

  buf_size = buf_size < capacity - size ? buf_size : capacity - size;

  std::memcpy(data + size, buf, buf_size);
  size += buf_size;

  uint8_t *section_start = data;
  while (section_start - data < capacity && section_start[0] != STUFFING_BYTE) {
    size_t avaiable_space = capacity - (section_start - data);

    if (auto section = PatSection::try_parse(section_start, avaiable_space)) {
      section->extract_programs(programs);
      section_start += section->full_length;

      end_reached = true;
      continue;
    } else {
      end_reached = false;
      break;
    }
  }
}
