#ifndef MPEGTS_ANALYZER_SECTION_VIEW_H
#define MPEGTS_ANALYZER_SECTION_VIEW_H

#include <cstdint>
#include <memory>
#include <vector>

#include "../common.h"
#include "View.h"

struct SectionView {
 public:
  SectionView() = delete;

  SectionView(const SectionView &other) = delete;
  SectionView(SectionView &&other) = delete;

  SectionView &operator=(const SectionView &other) = delete;
  SectionView &operator=(SectionView &&other) = delete;

  uint8_t tid() const { return hdr[0]; }

  uint8_t SSI() const { return GET_BIT(hdr + 1, 7); }

  uint16_t section_length() const { return GET_SHORT(hdr + 1) & MASK(12); }

  uint16_t tid_ext() const { return GET_SHORT(hdr + 3); }

  uint8_t version_number() const { return GET_BITS(hdr + 5, 1, 5); }

  uint8_t CNI() const { return GET_BIT(hdr + 5, 0); }

  uint8_t section_number() const { return hdr[6]; }

  uint8_t last_section_number() const { return hdr[7]; }

  size_t size() const { return 3 + section_length(); }

  const uint8_t *start() const { return hdr; }

  const uint8_t *end() const { return hdr + size(); }

  static constexpr size_t HDR_SIZE = 8;

 private:
  uint8_t hdr[HDR_SIZE];
  uint8_t data[];
};

using SectionPtr = std::unique_ptr<SectionView>;

using ConstSectionIt = std::vector<SectionPtr>::const_iterator;

template <>
struct ViewTraits<SectionView> {
  static constexpr size_t MIN_SIZE = SectionView::HDR_SIZE + CRC32_SIZE;

  static size_t size(const SectionView *view) { return view->size(); }
};

#endif
