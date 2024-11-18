#include "AssemblyTracker.h"
#include <cstring>

AssemblyTracker::AssemblyTracker() : is_assembled(false), is_changed(false) {
  this->sections_amount = 0;
  std::memset(flags_segments, 0, sizeof(flags_segments));
}

size_t AssemblyTracker::calc_segments_amount() {
  return sections_amount % FLAGS_PER_SEGMENT == 0
             ? sections_amount / FLAGS_PER_SEGMENT
             : sections_amount / FLAGS_PER_SEGMENT + 1;
}

bool AssemblyTracker::assembled() {
  if (!is_changed) {
    return is_assembled;
  }

  size_t segments_amount = calc_segments_amount();

  for (size_t segment_i = 0; segment_i < segments_amount; segment_i++) {
    if (segment_i == segments_amount - 1 &&
        flags_segments[segment_i] ==
            (1 << (sections_amount % FLAGS_PER_SEGMENT)) - 1) {
      is_assembled = true;
      return is_assembled;
    }

    if (segment_i < segments_amount - 1 && flags_segments[segment_i] == ~0) {
      continue;
    }

    is_assembled = false;
    return is_assembled;
  }

  is_assembled = false;
  return is_assembled;
}

void AssemblyTracker::reset(size_t sections_amount) {
  if (sections_amount > MAX_SECTIONS_AMOUNT) {
    return;
  }

  this->sections_amount = sections_amount;
  is_changed = true;
  is_assembled = false;
  std::memset(flags_segments, 0, sizeof(flags_segments));
}

void AssemblyTracker::mark_received(size_t n) {
  if (n > sections_amount) {
    return;
  }

  is_changed = true;
  flags_segments[n / FLAGS_PER_SEGMENT] |= 1 << (n % FLAGS_PER_SEGMENT);
}

void AssemblyTracker::unmark_received(size_t n) {
  if (n > sections_amount) {
    return;
  }

  is_changed = true;
  flags_segments[n / FLAGS_PER_SEGMENT] &= ~(1 << (n % FLAGS_PER_SEGMENT));
  FlagsSegment a;
}
