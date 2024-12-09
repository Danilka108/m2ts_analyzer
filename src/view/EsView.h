#ifndef MPEGTS_ANALYZER_ELEMENTARY_STREAM_H
#define MPEGTS_ANALYZER_ELEMENTARY_STREAM_H

#include <cstddef>
#include <cstdint>

#include "../common.h"
#include "View.h"

class EsView {
 public:
  uint8_t stream_type() const { return hdr[0]; }

  PID pid() const { return GET_SHORT(hdr + 1) & MASK(13); }

  uint16_t info_size() const { return GET_SHORT(hdr + 3) & MASK(12); }

  const uint8_t *info_start() const { return data; }

  const uint8_t *info_end() const { return data + info_size(); }

  const size_t size() const { return HDR_SIZE + info_size(); }

  static constexpr size_t HDR_SIZE = 5;

 private:
  uint8_t hdr[HDR_SIZE];
  uint8_t data[];
};

template <>
struct ViewTraits<EsView> {
  static constexpr size_t MIN_SIZE = EsView::HDR_SIZE;

  static size_t size(const EsView *view) { return view->size(); }
};

#endif
