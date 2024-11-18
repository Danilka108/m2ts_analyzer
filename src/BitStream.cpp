#include "BitStream.h"
#include <cstring>

BitStream::BitStream(const Buffer &buf) : _buf(buf) {}

BitStream::BitStream(const BitStream &other)
    : _buf(other._buf), _byte_pos(other._byte_pos), _bit_pos(other._bit_pos) {}

BitStream &BitStream::operator=(const BitStream &other) {
  if (this == &other) {
    return *this;
  }

  _buf = other._buf;
  _byte_pos = other._byte_pos;
  _bit_pos = other._bit_pos;

  return *this;
}

int BitStream::get_bit() {
  if (_byte_pos == _buf.get().size()) {
    return -1;
  }

  uint8_t byte = *((uint8_t *)_buf.get().data() + _byte_pos);
  uint8_t bit = (byte >> (8 - _bit_pos)) & 1;

  if (++_bit_pos == 8) {
    _bit_pos = 0;
    _byte_pos += 1;
  }

  return bit;
}
