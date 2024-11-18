#include "PsiParser.h"

#include "SdtParser.h"
#include "SdtSection.h"

#include <cstdio>

PsiParser::PsiParser(uint16_t pid)
    : curr_version(-1),
      prev_cc(-1),
      size(0),
      end_reached(false),
      pid(pid) {
}

uint16_t PsiParser::get_pid() const {
  return pid;
}

void PsiParser::reset(size_t sections_amount, uint8_t curr_version) {
  this->curr_version = curr_version;
  prev_cc = -1;
  size = 0;
  end_reached = false;
  tracker.reset(sections_amount);
}

bool PsiParser::parse_packet(TsPacket &packet) {
  if (tracker.assembled()) {
    return false;
  }

  if (packet.pid != pid) {
    return false;
  }

  if (packet.payload_until_start_indicator) {
    size_t section_start = packet.data[0];
    if (packet.data + section_start > packet.end) {
      return true;
    }

    if (packet.is_continuous(prev_cc)) {
      write_packet_data(packet.data + 1, section_start, false);
    }

    write_packet_data(packet.data + 1 + section_start,
                      packet.data_len - 1 - section_start, true);
  } else {
    if (packet.is_continuous(prev_cc)) {
      write_packet_data(packet.data, packet.data_len, false);
    }
  }

  prev_cc = packet.continuity_counter;

  return true;
}

/*bool PsiParser::is_stopped() { return assembled; }*/

void PsiParser::write_packet_data(uint8_t *buf, size_t buf_size,
                                  bool reset_buf) {
  PsiSection section;
  size_t avaiable_space = 0;
  uint8_t *section_start = NULL;
  bool handled = false;

  if (reset_buf) {
    size = 0;
    end_reached = false;
  }

  if (end_reached) {
    return;
  }

  buf_size = buf_size < BUF_CAPACITY - size ? buf_size : BUF_CAPACITY - size;
  std::memcpy(data + size, buf, buf_size);
  size += buf_size;

  section_start = data;
  while (section_start - data < size && section_start[0] != STUFFING_BYTE) {
    avaiable_space = size - (section_start - data);

    if (PsiSection::try_parse(&section, section_start, avaiable_space)) {
      handled = handle_section(section);

      section_start += section.full_length;
      end_reached = true;

      if (handled && section.current_next_indicator != 1) {
        reset(0, -1);
        continue;
      }

      if (handled && curr_version == -1) {
        reset(section.last_section_number + 1, section.version_number);
      }

      if (handled && section.version_number != curr_version) {
        reset(section.last_section_number + 1, section.version_number);
        reset_info();
      }

      if (handled) {
        tracker.mark_received(section.section_number);
      }
    } else {
      end_reached = false;
      break;
    }
  }
}
