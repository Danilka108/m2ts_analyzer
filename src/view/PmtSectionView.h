#ifndef MPEGTS_ANALYZER_PMT_SECTION_VIEW_H
#define MPEGTS_ANALYZER_PMT_SECTION_VIEW_H

#include <cstddef>

#include "../common.h"
#include "SectionView.h"
#include "View.h"

struct PmtSectionView {
 public:
  SectionView base;

  PRG_NUM prg_num() const { return base.tid_ext(); }

  uint16_t info_size() const {
    return GET_SHORT(base.start() + HDR_SIZE - 2) & MASK(12);
  }

  const uint8_t *info_start() const { return base.start() + HDR_SIZE; }

  const uint8_t *info_end() const {
    return std::min(info_start() + info_size(), base.end());
  }

  const uint8_t *payload_start() const { return info_end(); }

  const uint8_t *payload_end() const { return base.end() - CRC32_SIZE; }

  size_t payload_size() const { return payload_end() - payload_start(); }

  size_t size() const { return base.size(); }

  static constexpr size_t HDR_SIZE = 12;
};

template <>
struct ViewTraits<PmtSectionView> {
  static constexpr size_t MIN_SIZE = PmtSectionView::HDR_SIZE + CRC32_SIZE;

  static size_t size(const PmtSectionView *view) { return view->size(); }
};

#endif
