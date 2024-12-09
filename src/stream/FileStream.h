#ifndef MPEGTS_ANALYZER_FILE_STREAM_H
#define MPEGTS_ANALYZER_FILE_STREAM_H

#include <cstddef>

#include "Stream.h"

class FileStream final: public Stream {
 public:
  FileStream(const char *filename);

  FileStream(const FileStream &) = delete;
  FileStream &operator=(const FileStream &) = delete;

  bool is_valid() const override;

  ~FileStream() override;

 protected:
  size_t read_to_buf(uint8_t *buf, size_t buf_size) override;

 private:
  int fd;
};

#endif
