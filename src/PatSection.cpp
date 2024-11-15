#include "PatSection.h"
#include "Context.h"
#include "Program.h"
#include "common.h"
#include <cstdio>
#include <iostream>
#include <optional>
#include <ostream>
#include <vector>

PatSection::PatSection(uint8_t *buf)
    : table_id(*buf), section_syntax_indicator(GET_BIT(buf + 1, 7)),
      zero_field(GET_BIT(buf + 1, 6)),
      section_length(GET_SHORT(buf + 1) & MASK(12)),
      version_number(GET_BITS(buf + 5, 1, 5)),
      transport_stream_id(GET_SHORT(buf + 3)),
      current_next_indicator(GET_BIT(buf + 5, 0)), section_number(buf[6]),
      last_section_number(buf[7]), start(buf), data_start(buf + 8),
      crc_32(buf + 3 + section_length - 4), full_length(3 + section_length) {}

bool PatSection::is_valid() {
  return table_id == TABLE_ID && current_next_indicator == 1 &&
         section_syntax_indicator == 1 && zero_field == 0 &&
         full_length <= MAX_SIZE;
}

void PatSection::extract_info(Context &ctx) {
  if (!is_valid()) {
    return;
  }

  uint8_t *prg_num_start = data_start;
  uint16_t prg_num = 0;
  uint16_t pid = 0;
  bool already_exist = false;

  for (; prg_num_start < crc_32; prg_num_start += 4) {
    prg_num = GET_SHORT(prg_num_start);
    pid = GET_SHORT(prg_num_start + 2) & MASK(13);

    if (prg_num == 0) {
      continue;
    }

    /*already_exist = false;*/
    /*for (auto &program : ctx.programs) {*/
    /*  if (program.id == prg_num) {*/
    /*    program.map_pid = pid;*/
    /*    already_exist = true;*/
    /*    break;*/
    /*  }*/
    /*}*/

    /*if (!already_exist) {*/
    if (1) {
      /*std::cout << "program " << prg_num << " map_pid " << pid << std::endl;*/
      Program new_program(prg_num, pid);
      ctx.programs.push_back(new_program);
    }
  }
}

std::optional<PatSection> PatSection::try_parse(uint8_t *buf, size_t buf_size) {
  if (buf_size < REQUIRED_MIN_SIZE) {
    return {};
  }

  PatSection seg(buf);
  if (seg.full_length > buf_size) {
    return {};
  }

  return {seg};
}

bool PatSection::is_matches(Context &ctx, TsPacket &packet) {
  return packet.pid == PAT_PID;
}
