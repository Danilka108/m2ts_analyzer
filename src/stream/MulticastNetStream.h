#ifndef MPEGTS_ANALYZER_MULTICAST_NET_STREAM
#define MPEGTS_ANALYZER_MULTICAST_NET_STREAM

#include <netinet/in.h>

#include <cstddef>
#include <cstdint>

#include "Stream.h"

class MulticastNetStream final: public Stream {
 public:
  MulticastNetStream(struct sockaddr_in addr);

  MulticastNetStream(const MulticastNetStream &) = delete;
  MulticastNetStream &operator=(const MulticastNetStream &) = delete;

  bool is_valid() const override;

  ~MulticastNetStream() override;

 protected:
  size_t read_to_buf(uint8_t *buf, size_t buf_size) override;

 private:
  ip_mreq multicast_req {};
  int fd;
};

#endif
