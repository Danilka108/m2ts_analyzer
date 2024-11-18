#ifndef M2TS_ANALYZER_BITSTREAM_H
#define M2TS_ANALYZER_BITSTREAM_H

#include "Buffer.h"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>

template <typename I, size_t N>
using has_enough_bits_t = typename std::enable_if<(N < 8 * sizeof(N)), I>::type;

template <typename I>
using is_int_t = typename std::enable_if<std::is_integral<I>::value, I>::type;

class BitStream {
public:
  explicit BitStream(const Buffer &buf);

  BitStream(const BitStream &other);

  BitStream(BitStream &&other) = delete;

  BitStream &operator=(const BitStream &other);

  BitStream &operator=(BitStream &&other) = delete;

  ~BitStream() = default;

  int get_bit();

  template <size_t N, typename I = uint8_t, is_int_t<I> = true,
            has_enough_bits_t<I, N> = true>
  I get_bits() {
    uint8_t *start = data() + _byte_pos;

    if (_byte_pos + ((_bit_pos + N) / 8) >= size()) {
      return -1;
    }

    I result = 0;
    size_t offset = 0;
    while (offset < N) {
      uint8_t byte_offset = (_bit_pos + offset) / 8;
      uint8_t bit_offset = (_bit_pos + offset) % 8;

      uint8_t bits_amount = std::min<uint8_t>(8 - bit_offset, N - offset);

      uint8_t byte = *(start + byte_offset);
      uint8_t mask = (1 << bits_amount) - 1;
      uint8_t bits = (byte >> (8 - bit_offset - bits_amount)) & mask;

      result |= static_cast<I>(bits) << (N - offset - bits_amount);
      offset += bits_amount;
    }

    _byte_pos += offset / 8;
    _bit_pos += offset % 8;
    if (_bit_pos == 8) {
      _byte_pos += 1;
      _bit_pos = 0;
    }

    return result;
  }

private:
  inline uint8_t *data() { return (uint8_t *)_buf.get().data(); }

  inline size_t size() { return _buf.get().size(); }

  std::reference_wrapper<const Buffer> _buf;
  size_t _byte_pos;
  uint8_t _bit_pos;
};

#endif
