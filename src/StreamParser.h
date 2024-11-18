#ifndef M2TS_ANALYZER_PACKETS_HANDLER_H
#define M2TS_ANALYZER_PACKETS_HANDLER_H

#include "Context.h"
#include "PatParser.h"
#include "PmtParser.h"
#include "SdtParser.h"
#include "TsPacket.h"
#include <ostream>

class StreamParser {
public:
  StreamParser();

  void parse_packet(TsPacketBuf buf);

  /*bool is_stopped();*/

  friend std::ostream &operator<<(std::ostream &, const StreamParser &);

private:
  Context ctx;

  PatParser pat_parser;
  SdtParser sdt_parser;

  bool stopped;
};

std::ostream &operator<<(std::ostream &os, const StreamParser &parser);

#endif
