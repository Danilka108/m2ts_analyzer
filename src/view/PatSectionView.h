#ifndef MPEGTS_ANALYZER_PAT_SECTION_VIEW_H
#define MPEGTS_ANALYZER_PAT_SECTION_VIEW_H

#include <cstddef>

#include "../common.h"
#include "SectionView.h"
#include "View.h"

struct PatSectionView {
 public:
  SectionView base;

  TSID tsid() const { return base.tid_ext(); }

  const uint8_t *payload_start() const { return base.start() + HDR_SIZE; }

  const uint8_t *payload_end() const { return base.end() - CRC32_SIZE; }

  size_t payload_size() const { return payload_end() - payload_start(); }

  size_t size() const { return base.size(); }

  static constexpr size_t HDR_SIZE = 8;
};

template <>
struct ViewTraits<PatSectionView> {
  static constexpr size_t MIN_SIZE = PatSectionView::HDR_SIZE + CRC32_SIZE;

  static size_t size(const PatSectionView *view) { return view->size(); }
};

#endif
