/*#include <fcntl.h>*/
/*#include <limits.h>*/
/*#include <stdio.h>*/
/*#include <stdlib.h>*/
/*#include <string.h>*/
/*#include <sys/types.h>*/
/*#include <unistd.h>*/

#include "ts_packet.h"
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
/*#include <unistd.h>*/

int main(int argc, char *argv[]) {
  int fd;
  ts_packet_buf buf;
  size_t buf_size = 0;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  while (1) {
    buf_size = read(fd, buf.data, sizeof(buf.data));
    if (buf_size == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }

    if (buf_size == 0) {
      break;
    }

    if (buf_size != TS_PACKET_SIZE) {
      fprintf(stderr, "read less than ts packet size (%lu bytes)\n", buf_size);
      break;
    }

    ts_packet packet(&buf);
    /*if (!packet.is_valid()) {*/
    /*  continue;*/
    /*}*/

    if (packet.pid == 0) {
      printf(
          "pid 0 afc = %d, tei = %d, continuity counter = %d, is start = %d\n",
          packet.adaptation_field_control, packet.transport_error_indicator,
          packet.continuity_counter, packet.payload_until_start_indicator);
    }
  }

  return 0;
}
