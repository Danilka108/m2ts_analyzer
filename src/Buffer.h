#ifndef M2TS_ANALYZER_BUFFER_H
#define M2TS_ANALYZER_BUFFER_H

#include <cstddef>

class Buffer {
public:
  explicit Buffer(size_t size);

  Buffer(std::nullptr_t);

  Buffer(const Buffer &other) = delete;

  Buffer(Buffer &&other);

  Buffer &operator=(const Buffer &other) = delete;

  Buffer &operator=(Buffer &&other);

  ~Buffer();

  inline void *data() const {
    return _data;
  }

  inline size_t size() const {
    return _size;
  }

private:
  void *_data;
  size_t _size;
};

#endif
