#include "PsiSection.h"

#include "common.h"

PsiSection::PsiSection(uint8_t *buf)
    : table_id(buf[0]),
      section_syntax_indicator(GET_BIT(buf + 1, 7)),
      zero_field(GET_BIT(buf + 1, 6)),
      section_length(GET_SHORT(buf + 1) & MASK(12)),
      version_number(GET_BITS(buf + 5, 1, 5)),
      current_next_indicator(GET_BIT(buf + 5, 0)),
      section_number(buf[6]),
      last_section_number(buf[7]),
      buf(buf),
      crc_32(buf + 3 + section_length - 4),
      full_length(3 + section_length) {
}

PsiSection::PsiSection()
    : buf(nullptr),
      crc_32(nullptr) {
}

bool PsiSection::try_parse(PsiSection *section, uint8_t *buf, size_t buf_size) {
  if (buf_size < PsiSection::MIN_REQUIRED_SPACE) {
    return false;
  }

  PsiSection new_section(buf);
  if (new_section.full_length > buf_size) {
    return false;
  }

  *section = new_section;
  return true;
}
