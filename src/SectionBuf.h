#ifndef M2TS_ANALYZER_SECTION_BUF_H
#define M2TS_ANALYZER_SECTION_BUF_H

#include "Section.h"

#include <cstddef>
#include <cstdint>

class SectionBuf {
public:
  SectionBuf();

  SectionBuf(std::nullptr_t);

  SectionBuf(const SectionBuf &) = delete;
  SectionBuf(SectionBuf &&) = delete;

  SectionBuf &operator=(const SectionBuf &) = delete;
  SectionBuf &operator=(SectionBuf &&) = delete;

  Section *get() const;

private:
  static constexpr size_t CAPACITY = 4096;

  uint8_t *data;
  size_t size;
};

#endif
