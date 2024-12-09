#include <arpa/inet.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "UnicastNetStream.h"

UnicastNetStream::UnicastNetStream(sockaddr_in sock_addr) {
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    perror("Failed to create socket");
    return;
  }

  if (bind(fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) == -1) {
    perror("Binding failed");
    close(fd);
    fd = -1;
    return;
  }
}

bool UnicastNetStream::is_valid() const { return fd != -1; }

size_t UnicastNetStream::read_to_buf(uint8_t *buf, size_t buf_size) {
  sockaddr_in sender_addr {};
  socklen_t addr_len = sizeof(sender_addr);
  ssize_t size = recvfrom(fd, buf, buf_size, 0, (struct sockaddr *)&sender_addr,
                          &addr_len);

  if (size < 0) {
    perror("Receiving data failed");
    return 0;
  }

  return size;
}

UnicastNetStream::~UnicastNetStream() {
  if (fd != -1) {
    close(fd);
  }
}
