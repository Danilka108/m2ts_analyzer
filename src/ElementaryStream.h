#ifndef M2TS_ANALYZER_ELEMENTARY_STREAM_H
#define M2TS_ANALYZER_ELEMENTARY_STREAM_H

#include <cstddef>
#include <cstdint>

struct Stream;

enum StreamClass {
  STREAM_VIDEO_CLASS,
  STREAM_AUDIO_CLASS,
  STREAM_SUBTITLE_CLASS,
  STREAM_UNKNOWN_CLASS,
};

struct ElementaryStream {
public:
  ElementaryStream(uint8_t *buf);

  void extract_info(Stream &stream);

  uint8_t stream_type;
  uint16_t elementary_pid;
  uint16_t es_info_length;
  size_t full_length;

  uint8_t *data;
};

#endif
