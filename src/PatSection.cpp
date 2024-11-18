#include "PatSection.h"
#include "Context.h"
#include "PsiSection.h"
#include "common.h"

PatSection::PatSection(PsiSection &psi_section)
    : PsiSection(psi_section), transport_stream_id(GET_SHORT(buf + 3)),
      data(buf + 8) {}

bool PatSection::is_valid() {
  return table_id == TABLE_ID && section_syntax_indicator == 1 &&
         zero_field == 0 && full_length <= MAX_SIZE;
}

void PatSection::extract_info(Context &ctx) {
  uint8_t *prg_num_start = data;
  uint16_t prg_num = 0;
  uint16_t pid = 0;

  for (; prg_num_start < crc_32; prg_num_start += 4) {
    prg_num = GET_SHORT(prg_num_start);
    pid = GET_SHORT(prg_num_start + 2) & MASK(13);

    if (prg_num == 0) {
      continue;
    }

    ctx.programs.emplace_front(prg_num, pid);
    volatile int b = 0;
  }
}
