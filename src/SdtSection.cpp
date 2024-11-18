#include "SdtSection.h"
#include "Context.h"
#include "PsiSection.h"
#include "ServiceDescription.h"
#include "common.h"

SdtSection::SdtSection(PsiSection &psi_section)
    : PsiSection(psi_section), transport_stream_id(GET_SHORT(buf + 3)),
      original_network_id(GET_SHORT(buf + 8)), data(buf + 11) {
  zero_field = 0;
}

bool SdtSection::is_valid() {
  return table_id == TABLE_ID_ACTUAL_TS ||
         table_id == TABLE_ID_OTHER_TS && section_syntax_indicator == 1 &&
             zero_field == 0 && full_length <= MAX_SIZE &&
             section_number == 0 && last_section_number == 0;
}

void SdtSection::extract_info(Context &ctx) {
  uint8_t *s_info_start = data;

  while (s_info_start < crc_32) {
    ServiceDescription s_desc(s_info_start);
    s_desc.extract_info(ctx);

    s_info_start += s_desc.full_length;
  }
}
