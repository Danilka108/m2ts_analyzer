#ifndef MPEGTS_ANALYZER_UNICAST_NET_STREAM
#define MPEGTS_ANALYZER_UNICAST_NET_STREAM

#include <netinet/in.h>

#include <cstddef>
#include <cstdint>

#include "Stream.h"

class UnicastNetStream final: public Stream {
 public:
  UnicastNetStream(sockaddr_in sock_addr);

  UnicastNetStream(const UnicastNetStream &) = delete;
  UnicastNetStream &operator=(const UnicastNetStream &) = delete;

  bool is_valid() const override;

  ~UnicastNetStream() override;

 protected:
  size_t read_to_buf(uint8_t *buf, size_t buf_size) override;

 private:
  int fd;
};

#endif
