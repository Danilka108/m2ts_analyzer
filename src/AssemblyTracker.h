#ifndef M2TS_ANALYZER_SECTIONS_TRACKER_H
#define M2TS_ANALYZER_SECTIONS_TRACKER_H

#include <cstddef>
#include <cstdint>

class AssemblyTracker {
public:
  AssemblyTracker();

  bool assembled();

  void reset(size_t sections_amount);

  void mark_received(size_t n);

  void unmark_received(size_t n);

private:
  using FlagsSegment = uint64_t;

  size_t calc_segments_amount();

  static constexpr size_t MAX_SECTIONS_AMOUNT = sizeof(uint8_t);

  static constexpr size_t FLAGS_PER_SEGMENT = sizeof(FlagsSegment) * 8;

  static constexpr size_t MAX_SEGMENTS_AMOUNT =
      MAX_SECTIONS_AMOUNT % FLAGS_PER_SEGMENT == 0
          ? MAX_SECTIONS_AMOUNT / FLAGS_PER_SEGMENT
          : MAX_SECTIONS_AMOUNT / FLAGS_PER_SEGMENT + 1;

  FlagsSegment flags_segments[MAX_SEGMENTS_AMOUNT];
  size_t sections_amount;
  bool is_assembled;
  bool is_changed;
};

#endif
