#include "StreamParser.h"

#include "ElementaryStream.h"
#include "SdtParser.h"
#include "TsPacket.h"

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <iostream>

StreamParser::StreamParser()
    : ctx(),
      pat_parser(ctx),
      sdt_parser(ctx),
      stopped(false) {}

void StreamParser::parse_packet(TsPacketBuf buf) {
  TsPacket packet(buf);

  /*if (stopped) {*/
  /*  return;*/
  /*}*/

  if (!packet.is_valid()) {
    return;
  }

  if (pat_parser.parse_packet(packet)) {
    return;
  }

  if (sdt_parser.parse_packet(packet)) {
    return;
  }

  for (auto &prg : ctx.programs) {
    if (prg.pmt_parser.parse_packet(packet)) {
      return;
    }
  }
}

/*bool StreamParser::is_stopped() { return stopped; }*/

const char *get_stream_class(const Stream &stream) {
  switch (stream.stream_class) {
  case STREAM_AUDIO_CLASS:
    return "audio";
  case STREAM_VIDEO_CLASS:
    return "video";
  case STREAM_SUBTITLE_CLASS:
    return "subtitle";
  default:
    return "unknown";
    break;
  }
}

std::ostream &operator<<(std::ostream &os, const StreamParser &parser) {
  for (auto &component : parser.ctx.components) {
    os << "component " << (unsigned int)component.component_tag << std::endl;
    os << "  description:" << component.description << std::endl;
  }

  for (auto &program : parser.ctx.programs) {
    os << "program " << program.id << ":" << std::endl;
    os << " pmt pid: " << std::hex << program.pmt_parser.get_pid() << std::dec
       << std::endl;

    os << "  metadata:" << std::endl;
    if (program.description != nullptr) {
      os << "    description: " << program.description << std::endl;
    }
    if (program.name != nullptr) {
      os << "    name: " << program.name << std::endl;
    }
    if (program.provider_name != nullptr) {
      os << "    provider name: " << program.provider_name << std::endl;
    }

    for (const auto &stream : program.streams) {
      os << "  stream " << std::hex << stream.pid << std::dec << " "
         << stream.type << ":" << get_stream_class(stream) << std::endl;
    }
  }

  return os;
}
