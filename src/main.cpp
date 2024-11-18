#include "StreamParser.h"
#include "TsPacket.h"
#include "common.h"

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
  uint8_t buf[32768];
  int fd;
  size_t read_bytes = 0;
  size_t offset = 0;
  TsPacketBuf packet_buf;
  StreamParser parser;

  if (argc < 2) {
    fprintf(stderr, "usage: %s file_name\n", argv[0]);
  }

  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  /*Buffer buffer(32768);*/

  int i = 0;
  while (1) {
    /*if (parser.is_stopped()) {*/
    /*  break;*/
    /*}*/

    size_t unhandled_bytes_amount = read_bytes - offset;
    std::memmove(buf, buf + offset, unhandled_bytes_amount);
    read_bytes = read(fd, (uint8_t *)buf + unhandled_bytes_amount,
                      sizeof(buf) - unhandled_bytes_amount);

    if (read_bytes == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    /*BitStream bit_stream(buffer);*/

    if (read_bytes < TS_PACKET_SIZE) {
      break;
    }

    for (offset = 0; read_bytes - offset >= TS_PACKET_SIZE;) {
      if (buf[offset] == 0x47) {
        packet_buf.data = buf + offset;
        parser.parse_packet(packet_buf);
        offset += TS_PACKET_SIZE;
      } else {
        offset++;
      }
    }
  }

  std::cout << parser;

  exit(EXIT_SUCCESS);
}
