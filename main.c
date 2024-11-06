#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SYNC_BYTE 'G'

#define CONTROL_FIELDS_OFFSET ((8 + 1 + 1 + 1 + 13 + 2) / 8)

#define TP_HDR_SIZE 4

#define TP_DATA_SIZE 184

#define TP_SIZE (TP_HDR_SIZE + TP_DATA_SIZE)

#define PID_MASK ((unsigned short)((~0) & (~(0b111 << 13))))

int main(int argc, char *argv[]) {
  char buf[TP_SIZE];
  size_t buf_size = 0;
  int fd;

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
    buf_size = read(fd, buf, sizeof(buf));
    if (buf_size == -1) {
      perror("read");
      exit(EXIT_FAILURE);
    }
    if (buf_size < TP_SIZE) {
      printf("buf size < TP_SIZE\n");
      return 0;
    }

    unsigned short pid = *(unsigned short *)(buf + 1) & PID_MASK;
    /*if (pid == 0) {*/
    /*}*/
  }

  return 0;
}
