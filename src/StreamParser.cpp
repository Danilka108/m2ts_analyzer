#include "StreamParser.h"

#include <memory>

void StreamParser::parse() {
  const PacketView *packet = nullptr;

CYCLE:
  packet = stream->read_packet();

  if (!packet || !packet->is_valid()) {
    goto CYCLE;
  }

  ctx.psi_demuxer.process_packet(packet);

  if (ctx.pat.is_deserialized() && ctx.pat.is_applicable()) {
    for (auto &pmt : ctx.pmts) {
      if (!pmt.second.is_deserialized() || !pmt.second.is_applicable()) {
        goto CYCLE;
      }
    }

    return;
  }

  goto CYCLE;
}
