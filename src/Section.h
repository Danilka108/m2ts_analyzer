#ifndef M2TS_ANALYZER_SECTION_H
#define M2TS_ANALYZER_SECTION_H

#include "common.h"

#include <cstddef>
#include <cstdint>

class Section {
public:
  Section() = delete;

  /*
  Section(uint8_t *hdr, size_t hdr_size);

  Section(const Section &other) = delete;
  Section(Section &&other) = delete;

  Section &operator=(const Section &other) = delete;
  Section &operator=(Section &&other) = delete;
  */

  uint8_t get_table_id() const {
    return hdr[0];
  }

  uint8_t get_SSI() const {
    return GET_BIT(hdr + 1, 7);
  }

  uint16_t get_section_length() const {
    return GET_SHORT(hdr + 1) & MASK(12);
  }

  size_t get_full_length() const {
    return 3 + get_section_length();
  }

  uint16_t get_table_id_ext() const {
    return GET_SHORT(hdr + 3);
  }

  uint8_t get_version_number() const {
    return GET_BITS(hdr + 5, 1, 5);
  }

  uint8_t get_CNI() const {
    return GET_BIT(hdr + 5, 0);
  }

  uint8_t get_section_number() const {
    return hdr[6];
  }

  uint8_t get_last_section_number() const {
    return hdr[7];
  }

  static constexpr size_t HDR_SIZE = 8;

private:
  uint8_t hdr[HDR_SIZE];
  uint8_t data[];
  /*uint8_t *hdr;*/
  /*size_t hdr_size;*/
};

#endif
