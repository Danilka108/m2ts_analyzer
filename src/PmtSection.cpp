#include "PmtSection.h"
#include "TsPacket.h"
#include "common.h"
#include <cstdio>

PmtSection::PmtSection(uint8_t *buf)
    : table_id(buf[0]), section_syntax_indicator(GET_BIT(buf + 1, 7)),
      zero_field(GET_BIT(buf + 1, 6)),
      section_length(GET_SHORT(buf + 1) & MASK(12)),
      program_number(GET_SHORT(buf + 3)),
      version_number(GET_BITS(buf + 5, 1, 5)),
      current_next_indicator(GET_BIT(buf + 5, 0)), section_number(buf[6]),
      last_section_number(buf[7]), start(buf),
      program_info_length(GET_SHORT(buf + 10) & MASK(12)),
      data_start(start + 12 + program_info_length),
      crc_32(start + 3 + section_length - 4), full_length(3 + section_length) {}

bool PmtSection::is_valid() {
  return table_id == TABLE_ID && section_syntax_indicator == 1 &&
         zero_field == 0 && full_length <= MAX_SIZE &&
         (program_info_length >> 10) == 0;
}

std::optional<PmtSection> PmtSection::try_parse(uint8_t *buf, size_t buf_size) {
  if (buf_size < REQUIRED_MIN_SIZE) {
    return {};
  }

  PmtSection pmt(buf);
  if (pmt.full_length > buf_size) {
    return {};
  }

  return {pmt};
}

void PmtSection::extract_info(Context &ctx) {
  if (!is_valid()) {
    return;
  }

  uint8_t *stream_info_start = data_start;
  uint8_t stream_type = 0;
  uint16_t elementary_pid = 0;
  uint16_t es_info_length = 0;
  Program *program = nullptr;

  for (auto &prg : ctx.programs) {
    if (prg.id == program_number) {
      program = &prg;
      break;
    }
  }

  if (program == nullptr) {
    return;
  }

  program->streams.clear();

  while (stream_info_start < crc_32) {
    stream_type = stream_info_start[0];
    elementary_pid = GET_SHORT(stream_info_start + 1) & MASK(13);
    es_info_length = GET_SHORT(stream_info_start + 3) & MASK(12);
    stream_info_start += 5 + es_info_length;

    Stream stream(stream_type, elementary_pid);
    program->streams.push_back(stream);
  }

  program->pmt_found = true;
}

bool PmtSection::is_matches(Context &ctx, TsPacket &packet) {
  for (auto &program : ctx.programs) {
    if (program.map_pid == packet.pid && !program.pmt_found) {
      return true;
    }
  }

  return false;
}
