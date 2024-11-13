#include "PacketsHandler.h"
#include "common.h"
#include "ts_packet.h"
#include <cstring>
#include <string.h>
#include <vector>

PacketsHandler::PacketsHandler() : programs(), section_buf(programs) {}

void PacketsHandler::handle(ts_packet_buf &buf) {
  ts_packet packet(buf);

  if (!packet.is_valid()) {
    return;
  }

  if (packet.pid == PAT_PID) {
    handle_pat(packet);
    return;
  }
}

void PacketsHandler::handle_pat(ts_packet &packet) {
  if (packet.payload_until_start_indicator) {
    size_t section_start = 1 + packet.data[0];
    if (packet.data + section_start > packet.end) {
      return;
    }

    section_buf.insert(packet.data + 1, section_start);
    section_buf.reset();

    section_buf.insert(packet.data + section_start,
                       packet.data_len - section_start);
  } else {
    section_buf.insert(packet.data, packet.end - packet.data);
  }
}
