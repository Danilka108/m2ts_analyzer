#ifndef M2TS_ANALYZER_PAT_SECTION_H
#define M2TS_ANALYZER_PAT_SECTION_H

#include "Program.h"
#include "ts_packet.h"
#include <cstdint>
#include <optional>
#include <vector>

struct PatSection {
public:
  static constexpr size_t required_min_space = 12;

  const uint8_t table_id;
  const uint8_t section_syntax_indicator;
  const uint16_t section_length;
  const uint16_t transport_stream_id;
  const uint8_t current_next_indicator;
  const uint8_t section_number;
  const uint8_t last_section_number;

  uint8_t *const hdr_start;
  uint8_t *const data_start;
  uint8_t *const crc_32;

  const uint16_t full_length;

  void extract_programs(std::vector<Program> &programs);

  static std::optional<PatSection> try_parse(uint8_t *buf, size_t buf_size);

private:
  bool is_valid();

  PatSection(uint8_t *buf);
};

#endif
