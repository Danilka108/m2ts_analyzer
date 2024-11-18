#ifndef M2TS_ANALYZER_TABLE_DEMUXER_H
#define M2TS_ANALYZER_TABLE_DEMUXER_H

#include "AssemblyTracker.h"
#include "Section.h"

#include <cstddef>
#include <cstdint>

class PsiDemuxer {
public:
  PsiDemuxer();

private:
  struct SectionBuf {};


  struct TableContext {
    AssemblyTracker tracker;
    uint8_t version;
  };

  struct PidContext {
    static constexpr size_t BUF_CAPACITY = 4096;

    uint8_t cc;

    uint8_t buf[BUF_CAPACITY];
    size_t buf_size;
  };
};

#endif
