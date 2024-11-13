#include "PatSection.h"
#include "Program.h"
#include "common.h"
#include <optional>
#include <vector>

PatSection::PatSection(uint8_t *buf)
    : table_id(*buf), section_syntax_indicator(GET_BIT(buf + 1, 7)),
      section_length(GET_SHORT(buf + 1) & MASK(12)),
      transport_stream_id(GET_SHORT(buf + 3)),
      current_next_indicator(GET_BIT(buf + 5, 0)), section_number(buf[6]),
      last_section_number(buf[7]), hdr_start(buf), data_start(buf + 8),
      crc_32(buf + 3 + section_length - 4), full_length(3 + section_length) {}

bool PatSection::is_valid() {
  return table_id == PAT_TABLE_ID && section_syntax_indicator == 1 &&
         full_length >= PAS_MAX_SIZE;
}

void PatSection::extract_programs(std::vector<Program> &programs) {
  if (!is_valid()) {
    return;
  }

  uint8_t *pn_start = data_start;
  uint16_t pn = 0;
  uint16_t pid = 0;

  for (; pn_start < data_start; pn_start += 4) {
    uint16_t pn = GET_SHORT(pn_start);
    pid = GET_SHORT(pn_start + 2) & MASK(13);

    if (pn != 0) {
      Program prg(pn, pid);
      programs.push_back(prg);
    }
  }
}

std::optional<PatSection> PatSection::try_parse(uint8_t *buf, size_t buf_size) {
  if (buf_size < required_min_space) {
    return {};
  }

  PatSection seg(buf);
  if (seg.full_length > buf_size) {
    return {};
  } else {
    return {seg};
  }
}
