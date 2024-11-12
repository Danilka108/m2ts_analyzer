#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "pat.h"
#include "pmt.h"

int main(int argc, char *argv[]) {
  int fd;
  ts_packet_buf buf;
  size_t buf_size = 0;

  programs_t programs;
  programs.next = NULL;

  pat_t pat;
  init_pat(&pat);

  pmt_t pmt;
  init_pmt(&pmt);

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
      /*fprintf(stderr, "read less than ts packet size (%lu bytes)\n",
       * buf_size);*/
      break;
    }

    if (read_pat(&pat, buf) == -1) {
      continue;
    }
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
      /*fprintf(stderr, "read less than ts packet size (%lu bytes)\n",
       * buf_size);*/
      break;
    }

    if (read_pmt(&pat, buf) == -1) {
      continue;
    }
  }

  if (extract_program_numbers(&programs, &pat) == -1) {
    fprintf(stderr, "failed to extract program numbers\n");
    exit(EXIT_FAILURE);
  }

  if (extract_programs_info(&pmt, &programs) == -1) {
    fprintf(stderr, "failed to extract programs info\n");
    exit(EXIT_FAILURE);
  }

  for (p_node_t *node = programs.next; node != NULL; node = node->next) {
    printf("program %hu:\n ", node->program_number);
    for (size_t stream_index = 0; stream_index < node->streams_size;
         stream_index++) {
      switch (node->streams[stream_index]) {
      case (0x01):
        printf(" ISO/IEC 11172 Video");
        break;
      case (0x02):
        printf(" ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 "
               "constrained parameter video stream");
        break;
      case (0x03):
        printf(" ISO/IEC 11172 Audio");
        break;
      case (0x04):
        printf(" ISO/IEC 13818-3 Audio");
        break;
      case (0x07):
        printf("ISO/IEC 13522 MHEG");
        break;
      case (0x08):
        printf("ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC");
        break;
      case (0x09):
        printf("ITU-T Rec. H.222.1");
        break;
      case (0x0A):
        printf("ISO/IEC 13818-6 type A");
        break;
      case (0x0B):
        printf("ISO/IEC 13818-6 type B");
        break;
      case (0x0C):
        printf("ISO/IEC 13818-6 type C");
        break;
      case (0x0D):
        printf("ISO/IEC 13818-6 type D");
        break;
      case (0x0E):
        printf("ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary");
        break;
      case (0x0F):
        printf("ISO/IEC 13818-7 Audio with ADTS transport syntax");
        break;
      case (0x10):
        printf("ISO/IEC 14496-2 Visual");
        break;
      case (0x11):
        printf("ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1");
        break;
      case (0x12):
        printf("ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets");
        break;
      case (0x13):
        printf("ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.");
        break;
      case (0x14):
        printf("ISO/IEC 13818-6 Synchronized Download Protocol");
        break;
      }
      printf("\n");
    }
  }

  free_pmt(&pmt);
  free_program_numbers(&programs);
  free_pat(&pat);

  return 0;
}
