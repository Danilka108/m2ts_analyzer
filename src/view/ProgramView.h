#ifndef MPEGTS_ANALYER_PROGRAM_VIEW_H
#define MPEGTS_ANALYER_PROGRAM_VIEW_H

#include <cstddef>
#include <cstdint>

#include "../common.h"
#include "View.h"
struct ProgramView {
  PRG_NUM prg_num() const { return GET_SHORT(hdr); }

  PID map_PID() const { return GET_SHORT(hdr + 2) & MASK(13); }

  static constexpr size_t HDR_SIZE = 4;

 private:
  uint8_t hdr[HDR_SIZE];
};

template <>
struct ViewTraits<ProgramView> {
  static constexpr size_t MIN_SIZE = ProgramView::HDR_SIZE;

  static size_t size(const ProgramView *view) { return ProgramView::HDR_SIZE; }
};

#endif
