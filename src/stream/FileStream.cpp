#include "FileStream.h"

#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

FileStream::FileStream(const char *filename) { fd = open(filename, O_RDONLY); }

bool FileStream::is_valid() const { return fd != -1; }

size_t FileStream::read_to_buf(uint8_t *buf, size_t buf_size) {
  ssize_t read_bytes = read(fd, buf, buf_size);
  if (read_bytes == -1) {
    perror("read");
    return 0;
  }

  return read_bytes;
}

FileStream::~FileStream() {
  if (fd != -1) {
    close(fd);
  }
}
