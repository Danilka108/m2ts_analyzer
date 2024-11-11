#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "pat.h"

int main(int argc, char *argv[]) {
  int fd;
  ts_packet_buf buf;
  size_t buf_size = 0;

  program_numbers_t program_numbers;
  program_numbers.next = NULL;

  pat_t pat;
  init_pat(&pat);

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

    if (buf_size == 0) {
      break;
    }

    if (buf_size != TS_PACKET_SIZE) {
      /*fprintf(stderr, "read less than ts packet size (%lu bytes)\n", buf_size);*/
      break;
    }

    if (read_pat(&pat, buf) == -1) {
      continue;
    }
  }

  if (extract_program_numbers(&program_numbers, &pat)) {
    fprintf(stderr, "failed to extract program numbers\n");
  }

  for (pn_node_t *node = program_numbers.next; node != NULL;
       node = node->next) {
    printf("transport stream: %hu, program %hu: program map pid = %hu\n", node->ts_id, node->program_number,
           node->program_map_pid);
  }

  free_program_numbers(&program_numbers);
  free_pat(&pat);

  return 0;
}
