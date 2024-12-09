#include "Indent.h"
#include "StreamParser.h"
#include "stream/FileStream.h"
#include "stream/MulticastNetStream.h"
#include "stream/UnicastNetStream.h"

#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <unistd.h>

bool try_parse_addr_port(const char *str, struct in_addr *addr,
                         uint16_t *port) {
  char addr_str[16];

  size_t i = 0;
  for (; i < sizeof(addr_str) - 1; i++) {
    if (str[i] == ':') {
      addr_str[i] = '\0';
      break;
    } else {
      addr_str[i] = str[i];
    }
  }

  if (sscanf(str + i + 1, "%hu", port) != 1) {
    return false;
  }

  if (inet_aton(addr_str, addr) == 0) {
    return false;
  }

  return true;
}

std::unique_ptr<Stream> try_init_net_stream(const char *sock_addr_str) {
  constexpr uint32_t lowest_multicast_addr = 0b00000000000000000000000011100000;
  constexpr uint32_t highest_multicast_addr =
      0b11111111111111111111111111101111;
  // 0b_00000000_00000000_00000000_11100000 // 224.000.000.000
  // 0b_11111111_11111111_11111111_11101111 // 239.255.255.255

  struct sockaddr_in sock_addr;
  sock_addr.sin_family = AF_INET;

  if (!try_parse_addr_port(sock_addr_str, &sock_addr.sin_addr,
                           &sock_addr.sin_port)) {
    return nullptr;
  }

  sock_addr.sin_port = htons(sock_addr.sin_port);

  std::unique_ptr<Stream> stream;

  if (sock_addr.sin_addr.s_addr >= lowest_multicast_addr &&
      sock_addr.sin_addr.s_addr <= highest_multicast_addr) {
    stream = std::unique_ptr<Stream>(new MulticastNetStream(sock_addr));
  } else {

    stream = std::unique_ptr<Stream>(new UnicastNetStream(sock_addr));
  }

  return stream->is_valid() ? std::move(stream) : nullptr;
}

std::unique_ptr<FileStream> try_init_file_stream(const char *filename) {
  auto stream = std::unique_ptr<FileStream>(new FileStream(filename));
  return stream->is_valid() ? std::move(stream) : nullptr;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s <file_name | ipv4_addr:port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  std::unique_ptr<Stream> stream = try_init_file_stream(argv[1]);

  if (stream == nullptr) {
    stream = try_init_net_stream(argv[1]);
  }

  if (stream == nullptr) {
    fprintf(stderr, "usage: %s <file_name | ipv4_addr:port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  StreamParser sp(std::move(stream));
  sp.parse();

  sp.print_programs(Indent<2>{}, std::cout);

  exit(EXIT_SUCCESS);
}
