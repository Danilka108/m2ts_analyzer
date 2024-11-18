#include "PmtSection.h"
#include "Context.h"
#include "Descriptor.h"
#include "ElementaryStream.h"
#include "common.h"

PmtSection::PmtSection(PsiSection &section)
    : PsiSection(section), program_number(GET_SHORT(buf + 3)),
      program_info_length(GET_SHORT(buf + 10) & MASK(12)), data(buf + 12) {}

bool PmtSection::is_valid() {
  return table_id == TABLE_ID && section_syntax_indicator == 1 &&
         zero_field == 0 && full_length <= MAX_SIZE &&
         (program_info_length >> 10) == 0 && section_number == 0 &&
         last_section_number == 0;
}

void PmtSection::extract_info(Program *prg) {
  uint8_t *prg_info_start = data;
  uint8_t *es_info_start = data + program_info_length;

  while (prg_info_start < es_info_start) {
    Descriptor desc(prg_info_start);
    prg_info_start += desc.full_length;
  }

  while (es_info_start < crc_32) {
    ElementaryStream es(es_info_start);
    prg->streams.emplace_front(es);

    es_info_start += es.full_length;
  }
}
