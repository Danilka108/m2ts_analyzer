#ifndef M2TS_ANALYZER_PAT_SECTION_H
#define M2TS_ANALYZER_PAT_SECTION_H

#include "PsiSection.h"
struct Context;

struct PatSection : public PsiSection {
public:
  PatSection(PsiSection &psi_section);

  bool is_valid();

  void extract_info(Context &ctx);

  static constexpr uint8_t TABLE_ID = 0x0;
  static constexpr size_t MAX_SIZE = 1024;
  static constexpr size_t REQUIRED_MIN_SIZE = 12;

  const uint16_t transport_stream_id;

  uint8_t *const data;
};

#endif
