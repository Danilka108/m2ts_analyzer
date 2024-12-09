#ifndef MPEGTS_ANALYZER_STREAM_H
#define MPEGTS_ANALYZER_STREAM_H

#include "../view/PacketView.h"

class Stream {
 public:
  const PacketView *read_packet();

  virtual ~Stream() {}

  virtual bool is_valid() const = 0;

 protected:
  virtual size_t read_to_buf(uint8_t *buf, size_t size) = 0;

 private:
  static constexpr size_t BUF_CAPACITY = 4096;

  size_t read_bytes;
  size_t offset;
  uint8_t buf[BUF_CAPACITY];
};

#endif
