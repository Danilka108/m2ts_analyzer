#include "MulticastNetStream.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

MulticastNetStream::MulticastNetStream(struct sockaddr_in addr) {
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd == -1) {
    perror("Failed to create socket");
    return;
  }

  sockaddr_in local_addr {};
  local_addr.sin_family = addr.sin_family;
  local_addr.sin_port = addr.sin_port;
  local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
    perror("Binding failed");
    close(fd);
    fd = -1;
    return;
  }

  multicast_req.imr_multiaddr.s_addr = addr.sin_addr.s_addr;
  multicast_req.imr_interface.s_addr = htonl(INADDR_ANY);

  /*int loop = 1;*/
  /*if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop)) ==*/
  /*    -1) {*/
  /*  perror("Failed to use looping of multicast traffic");*/
  /*  close(fd);*/
  /*  fd = -1;*/
  /*  return;*/
  /*}*/

  if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast_req,
                 sizeof(multicast_req)) < 0) {
    perror("Adding multicast group failed");
    close(fd);
    fd = -1;
    return;
  }
}

bool MulticastNetStream::is_valid() const { return fd != -1; }

size_t MulticastNetStream::read_to_buf(uint8_t *buf, size_t buf_size) {
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

MulticastNetStream::~MulticastNetStream() {
  if (fd == -1) {
    return;
  }

  if (setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicast_req,
                 sizeof(multicast_req)) < 0) {
    perror("Dropping multicast group failed");
  }

  close(fd);
}
