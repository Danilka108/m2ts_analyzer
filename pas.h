#ifndef M2TS_ANALYZER_PAS_H
#define M2TS_ANALYZER_PAS_H
//https://www.ffmpeg.org/doxygen/trunk/mpegts_8c.html#a9e5a92b28facd83bde720718f696be0b
#include <cstdint>

struct pas_buf {

};

struct pas {
  public:
    uint8_t table_id;
    uint8_t section_syntax_indicator;
    uint16_t section_length;
    uint16_t transport_stream_id;
    uint8_t current_next_indicator;
    uint8_t section_number;
    uint8_t last_section_number;
};

#endif
