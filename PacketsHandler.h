#ifndef M2TS_ANALYZER_PACKETS_HANDLER_H
#define M2TS_ANALYZER_PACKETS_HANDLER_H

#include "Program.h"
#include "SectionBuf.h"
#include "ts_packet.h"
#include <cstdint>
#include <vector>

class PacketsHandler {
public:
  PacketsHandler();

  void handle(ts_packet_buf &buf);

private:
  void handle_pat(ts_packet &packet);

  std::vector<Program> programs;
  SectionBuf section_buf;
};

#endif
