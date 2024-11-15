#ifndef M2TS_ANALYZER_PACKETS_HANDLER_H
#define M2TS_ANALYZER_PACKETS_HANDLER_H

#include "PatSection.h"
#include "PmtSection.h"
#include "Program.h"
#include "SectionHandler.h"
#include "TsPacket.h"
#include <cstdint>

class PacketsHandler {
public:
  PacketsHandler();

  void handle(TsPacketBuf &buf);

  bool are_all_tables_parsed();

  void print_programs();

private:
  Context ctx;

  SectionHandler<PatSection> pat_handler;
  SectionHandler<PmtSection> pmt_handler;

  int8_t prev_cc;
};

#endif
