#ifndef M2TS_ANALYZER_SDT_SECTION_H
#define M2TS_ANALYZER_SDT_SECTION_H

#include "PsiSection.h"

struct Context;

struct SdtSection : PsiSection {
public:
  SdtSection(PsiSection &psi_section);

  bool is_valid();

  void extract_info(Context &ctx);

  static constexpr uint8_t TABLE_ID_ACTUAL_TS = 0x42;
  static constexpr uint8_t TABLE_ID_OTHER_TS = 0x46;
  static constexpr size_t MAX_SIZE = 1024;
  static constexpr size_t REQUIRED_MIN_SIZE = 12;

  uint16_t transport_stream_id;
  uint16_t original_network_id;

  uint8_t *data;
};

#endif
