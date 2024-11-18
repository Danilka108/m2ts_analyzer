#include "SectionBuf.h"

#include <cstddef>
#include <new>

SectionBuf::SectionBuf()
    : data(static_cast<uint8_t *>(::operator new(CAPACITY))),
      size(CAPACITY) {
  if (data == nullptr) {
    size = 0;
  }
}

SectionBuf::SectionBuf(std::nullptr_t)
    : data(nullptr),
      size(0) {}

Section *SectionBuf::get() const {
  /*if (size < Section::MIN_SIZE) {*/
  /*  return nullptr;*/
  /*}*/

  return nullptr;
}
