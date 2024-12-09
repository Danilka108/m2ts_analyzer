#ifndef MPEGTS_ANALYZER_DESCRIPTOR_VIEW_H
#define MPEGTS_ANALYZER_DESCRIPTOR_VIEW_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "View.h"

struct DescriptorView {
 public:
  DescriptorView() = delete;

  uint8_t tag() const { return hdr[0]; }
  size_t payload_size() const { return hdr[1]; }

  const uint8_t *payload() const { return data; }

  size_t size() const { return HDR_SIZE + payload_size(); }

  bool is_subtitle() const;

  static constexpr size_t HDR_SIZE = 2;

 private:
  uint8_t hdr[HDR_SIZE];
  uint8_t data[];
};

using DescriptorPtr = std::shared_ptr<DescriptorView>;
using ConstDescriptorIt = std::vector<DescriptorPtr>::const_iterator;

template <>
struct ViewTraits<DescriptorView> {
  static constexpr size_t MIN_SIZE = DescriptorView::HDR_SIZE;

  static size_t size(const DescriptorView *view) { return view->size(); }
};

#endif
