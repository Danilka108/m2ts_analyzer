#ifndef MPEGTS_ANALYZER_PACKET_H
#define MPEGTS_ANALYZER_PACKET_H

#include <cstddef>
#include <cstdint>

#include "../common.h"
#include "../view/View.h"

class PacketView {
 public:
  PacketView() = delete;

  char sync_byte() const { return data[0]; }

  uint8_t tei() const { return GET_BIT(data + 1, 7); }

  uint8_t pusi() const { return GET_BIT(data + 1, 6); }

  PID pid() const { return GET_SHORT(data + 1) & MASK(13); }

  uint8_t afc() const { return GET_BITS(data + 3, 4, 6); }

  uint8_t cc() const { return GET_BITS(data + 3, 0, 4); }

  const uint8_t *payload() const {
    return data + HDR_SIZE + (afc() & 2 ? data[HDR_SIZE] : 0);
  }

  const uint8_t *end() const { return data + SIZE; }

  const uint8_t *crc() const { return data + SIZE - 4; }

  bool is_valid() const { return sync_byte() == SYNC_BYTE && tei() == 0; }

  bool is_continuous(int8_t last_cc) const {
    const uint8_t afc_value = afc(), *afc_start = data + HDR_SIZE;
    return (afc_value & 2) && afc_start[0] > 0 && ((afc_start[1] >> 7) & 1) ||
           last_cc < 0 ||
           (afc_value & 1 ? ((last_cc + 1) & 0x1FFF) : last_cc) == cc();
  }

  static constexpr size_t SIZE = 188;

  static constexpr size_t HDR_SIZE = 4;

  static constexpr char SYNC_BYTE = 0x47;

 private:
  uint8_t data[SIZE];
};

template <>
struct ViewTraits<PacketView> {
  static constexpr size_t MIN_SIZE = PacketView::SIZE;

  static size_t size(const PacketView *view) { return PacketView::SIZE; }
};

#endif
