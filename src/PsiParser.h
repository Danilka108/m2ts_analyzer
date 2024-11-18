#ifndef M2TS_ANALYZER_TABLE_PARSER_H
#define M2TS_ANALYZER_TABLE_PARSER_H

#include "AssemblyTracker.h"
#include "PsiSection.h"
#include "TsPacket.h"
#include <cstdint>
#include <cstring>

struct Context;

class PsiParser {
public:
  bool parse_packet(TsPacket &packet);

  uint16_t get_pid() const;

  bool table_assembled();

protected:
  PsiParser(uint16_t pid);

  virtual bool handle_section(PsiSection &section) = 0;

  virtual void reset_info() = 0;

private:
  void reset(size_t sections_amount, uint8_t curr_version);

  void write_packet_data(uint8_t *buf, size_t buf_size, bool reset);

  static constexpr uint8_t STUFFING_BYTE = 0xFF;
  static constexpr size_t BUF_CAPACITY = 4096;

  uint8_t data[BUF_CAPACITY];
  AssemblyTracker tracker;
  int16_t curr_version;
  int8_t prev_cc;
  size_t size;
  bool end_reached;

protected:
  uint16_t pid;
};

#endif
