#ifndef M2TS_ANALYZER_SECTION_H
#define M2TS_ANALYZER_SECTION_H

#include <cstddef>
#include <cstdint>
struct PsiSection {
public:
  PsiSection(uint8_t *buf);

  PsiSection();

  uint8_t table_id;
  uint8_t section_syntax_indicator;
  uint8_t zero_field;
  uint16_t section_length;
  uint8_t version_number;
  uint8_t current_next_indicator;
  uint8_t section_number;
  uint8_t last_section_number;

  uint8_t *buf;
  uint8_t *crc_32;

  size_t full_length;

  static constexpr size_t MIN_REQUIRED_SPACE = 8;

  static bool try_parse(PsiSection *section, uint8_t *buf, size_t buf_size);
};

#endif
