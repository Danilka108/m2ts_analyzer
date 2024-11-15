#ifndef M2TS_ANALYZER_SECTION_HANDLER_H
#define M2TS_ANALYZER_SECTION_HANDLER_H

#include "Context.h"
#include "TsPacket.h"
#include "common.h"
#include <concepts>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <optional>

template <typename S>
concept PSISection = requires(S s, uint8_t *buf, size_t buf_size, Context &ctx,
                              TsPacket &packet) {
  { s.extract_info(ctx) };
  { S::try_parse(buf, buf_size) } -> std::same_as<std::optional<S>>;
  { S::is_matches(ctx, packet) } -> std::same_as<bool>;
  { s.version_number } -> std::convertible_to<uint8_t>;
  { s.section_number } -> std::convertible_to<uint8_t>;
  { s.last_section_number } -> std::convertible_to<uint8_t>;
};

template <PSISection Section> class SectionHandler {
public:
  SectionHandler(Context &ctx)
      : ctx(ctx), last_pid(-1), sections_amount(0), curr_version(-1),
        prev_cc(-1), end_reached(0), size(0) {
    std::memset(sections_segments, 0, sizeof(sections_segments));
  }

  bool handle(TsPacket &packet) {
    if (!Section::is_matches(ctx, packet)) {
      return false;
    }

    if (last_pid == packet.pid && sections_amount > 0) {
      size_t accamulator = 0;
      for (size_t i = 0; i < sizeof(sections_segments) * 8; i++) {
        size_t a = (sizeof(*sections_segments) * 8);
        accamulator += (sections_segments[i / a] >> (i % a)) & 0x1;
      }
      if (accamulator == sections_amount) {
        if (packet.pid == 0) {
          ctx.all_pat_found = true;
        }
        return false;
      }
    }

    if (last_pid != packet.pid) {
      last_pid = packet.pid;
      curr_version = -1;
    }

    if (packet.payload_until_start_indicator) {
      size_t section_start = packet.data[0];
      if (packet.data + section_start > packet.end) {
        return true;
      }

      if (packet.is_continuous(prev_cc)) {
        insert(packet.data + 1, section_start);
        reset();
      }

      insert(packet.data + 1 + section_start,
             packet.data_len - 1 - section_start);
    } else {
      if (packet.is_continuous(prev_cc)) {
        insert(packet.data, packet.data_len);
      }
    }

    prev_cc = packet.continuity_counter;
    return true;
  }

private:
  void reset() {
    size = 0;
    end_reached = false;
  }

  void insert(uint8_t *buf, size_t buf_size) {
    if (end_reached) {
      return;
    }

    buf_size = buf_size < CAPACITY - size ? buf_size : CAPACITY - size;

    std::memcpy(data + size, buf, buf_size);
    size += buf_size;

    uint8_t *section_start = data;
    while (section_start - data < size && section_start[0] != STUFFING_BYTE) {
      size_t avaiable_space = size - (section_start - data);

      if (auto section = Section::try_parse(section_start, avaiable_space)) {
        section->extract_info(ctx);
        section_start += section->full_length;

        if (section->version_number != curr_version) {
          curr_version = section->version_number;
          sections_amount = section->last_section_number + 1;
          std::memset(sections_segments, 0, sizeof(sections_segments));
        }

        sections_segments[section->section_number / SEGMENT_SIZE] |=
            1 << (section->section_number % SEGMENT_SIZE);

        end_reached = true;
        continue;
      } else {
        end_reached = false;
        break;
      }
    }
  }

  static constexpr size_t CAPACITY = 4096;
  static constexpr size_t SECTIONS_SEGMENTS_SIZE = 4;
  static constexpr size_t SEGMENT_SIZE = 64;

  Context &ctx;

  uint32_t last_pid;
  uint16_t sections_amount;
  uint8_t curr_version;
  int8_t prev_cc;
  bool end_reached;
  size_t size;
  uint64_t sections_segments[SECTIONS_SEGMENTS_SIZE];
  uint8_t data[CAPACITY];
};

#endif
