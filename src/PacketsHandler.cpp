#include "PacketsHandler.h"
#include "TsPacket.h"
#include <cstring>
#include <iostream>

PacketsHandler::PacketsHandler()
    : pat_handler(ctx), pmt_handler(ctx), prev_cc(-1) {}

void PacketsHandler::handle(TsPacketBuf &buf) {
  TsPacket packet(buf);

  if (ctx.all_pat_found) {
    ctx.all_programs_info_found = true;
    for (auto &program : ctx.programs) {
      if (!program.pmt_found) {
        ctx.all_programs_info_found = false;
        break;
      }
    }

    if (ctx.all_pat_found && ctx.all_programs_info_found) {
      return;
    }
  }

  if (!packet.is_valid()) {
    return;
  }

  if (!ctx.all_pat_found && pat_handler.handle(packet)) {
    return;
  }

  if (pmt_handler.handle(packet)) {
    return;
  }

  return;
}

bool PacketsHandler::are_all_tables_parsed() {
  return ctx.all_pat_found && ctx.all_programs_info_found;
  /*return ctx.all_pat_found;*/
  /*return false;*/
}

void PacketsHandler::print_programs() {
  for (auto &program : ctx.programs) {
    std::cout << "program " << program.id << ":" << std::endl;
    std::cout << "  map_pid: " << program.map_pid << std::endl;
    for (auto &stream : program.streams) {
      std::cout << "  stream " << static_cast<unsigned>(stream.type) << ": "
                << std::endl;
    }
  }
}
