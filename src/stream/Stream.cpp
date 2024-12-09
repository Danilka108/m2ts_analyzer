#include "Stream.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

const PacketView *Stream::read_packet() {
  if (!is_valid()) {
    return nullptr;
  }

  while (1) {
    if (read_bytes - offset < PacketView::SIZE) {
      size_t unhandled_bytes_amount = read_bytes - offset;
      std::memmove(buf, buf + offset, unhandled_bytes_amount);
      offset = 0;

      read_bytes = read_to_buf((uint8_t *)buf + unhandled_bytes_amount,
                               BUF_CAPACITY - unhandled_bytes_amount);

      if (read_bytes < PacketView::SIZE) {
        return nullptr;
      }
    }

    for (; read_bytes - offset >= PacketView::SIZE;) {
      if (buf[offset] == PacketView::SYNC_BYTE) {
        PacketView *packet = reinterpret_cast<PacketView *>(buf + offset);
        offset += PacketView::SIZE;

        return packet;
      } else {
        offset++;
      }
    }
  }
}
