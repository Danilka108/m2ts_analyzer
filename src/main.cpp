#include "PacketsHandler.h"
#include "TsPacket.h"
#include "common.h"
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  uint8_t buf[32768];
  int fd;
  size_t read_bytes = 0;
  TsPacketBuf packet_buf;
  PacketsHandler packets_handler;
  size_t offset = 0;

  if (argc < 2) {
    fprintf(stderr, "usage: %s file_name\n", argv[0]);
  }

  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  while (1) {
    if (packets_handler.are_all_tables_parsed()) {
      break;
    }

    size_t unhandled_bytes_amount = read_bytes - offset;
    std::memmove(buf, buf + offset, unhandled_bytes_amount);
    read_bytes = read(fd, buf + unhandled_bytes_amount,
                      sizeof(buf) - unhandled_bytes_amount);
    if (read_bytes == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    for (offset = 0;;) {
      if (read_bytes - offset < TS_PACKET_SIZE) {
        break;
      }

      packet_buf.data = buf + offset;
      packets_handler.handle(packet_buf);
      offset += TS_PACKET_SIZE;
    }

    if (read_bytes < TS_PACKET_SIZE) {
      break;
    }
  }

  packets_handler.print_programs();

  exit(EXIT_SUCCESS);
}
