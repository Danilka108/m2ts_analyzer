#ifndef M2TS_ANALYZER_PAT_SECTION_H
#define M2TS_ANALYZER_PAT_SECTION_H

#include "Context.h"
#include "TsPacket.h"
#include <cstdint>
#include <optional>

struct PatSection {
public:
  static constexpr uint8_t TABLE_ID = 0x0;
  static constexpr size_t MAX_SIZE = 1024;
  static constexpr size_t REQUIRED_MIN_SIZE = 12;

  const uint8_t table_id;
  const uint8_t section_syntax_indicator;
  const uint8_t zero_field;
  const uint16_t section_length;
  const uint16_t transport_stream_id;
  const uint8_t version_number;
  const uint8_t current_next_indicator;
  const uint8_t section_number;
  const uint8_t last_section_number;

  uint8_t *const start;
  uint8_t *const data_start;
  uint8_t *const crc_32;

  const size_t full_length;

  void extract_info(Context &ctx);

  static std::optional<PatSection> try_parse(uint8_t *buf, size_t buf_size);

  static bool is_matches(Context &ctx, TsPacket &packet);

private:
  bool is_valid();

  PatSection(uint8_t *buf);
};

#endif
