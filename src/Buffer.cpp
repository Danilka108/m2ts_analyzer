#include "Buffer.h"
#include <cstddef>
#include <cstdlib>

Buffer::Buffer(size_t size) : _data(nullptr), _size(size) {
  _data = ::operator new(size);
  if (_data == nullptr) {
    std::exit(EXIT_FAILURE);
  }
}

Buffer::Buffer(std::nullptr_t) : _data(nullptr), _size(0) {}

Buffer::Buffer(Buffer &&other) : _data(other._data), _size(other._size) {
  other._data = nullptr;
  other._size = 0;
}

Buffer &Buffer::operator=(Buffer &&other) {
  if (_data != nullptr) {
    ::operator delete(_data);
  }

  _data = other._data;
  other._data = nullptr;

  _size = other._size;
  other._size = 0;

  return *this;
}

Buffer::~Buffer() {
  if (_data != nullptr) {
    ::operator delete(_data);
  }
}
