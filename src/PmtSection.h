#ifndef M2TS_ANALYZER_PMT_SECTION_H
#define M2TS_ANALYZER_PMT_SECTION_H

#include "PsiSection.h"

struct Program;

struct PmtSection : public PsiSection {
public:
  PmtSection(PsiSection &section);

  bool is_valid();

  void extract_info(Program *prg);

  static constexpr uint8_t TABLE_ID = 0x02;
  static constexpr size_t MAX_SIZE = 1024;
  static constexpr size_t REQUIRED_MIN_SIZE = 12;

  const uint16_t program_number;
  const uint16_t program_info_length;

  uint8_t *const data;
};

#endif
