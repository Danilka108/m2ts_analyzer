#ifndef MPEGTS_ANALYZER_PAT_TABLE_H
#define MPEGTS_ANALYZER_PAT_TABLE_H

#include <ostream>
#include <vector>

#include "../Indent.h"
#include "../common.h"
#include "../view/ProgramView.h"
#include "PmtTable.h"
#include "RawTable.h"
#include "Table.h"

struct Context;

class PatTable final: public Table {
 public:
  class Program {
   public:
    PRG_NUM number() const { return _number; }

    PID pid() const { return _pid; }

    bool deserialize(const ProgramView *view);

    template <size_t S>
    void print(PmtTable::ConstStreamIt streams_begin,
               PmtTable::ConstStreamIt streams_end, Indent<S> indent,
               std::ostream &os) const;

   private:
    PRG_NUM _number;
    PID _pid;
  };

  PatTable() {}

  PatTable(const PatTable &) = delete;
  PatTable &operator=(const PatTable &) = delete;

  std::vector<Program>::const_iterator programs_begin() const {
    return _programs.begin();
  }

  std::vector<Program>::const_iterator programs_end() const {
    return _programs.end();
  }

  static constexpr TID ID = 0x00;

 private:
  bool deserialize_payload(const RawTable &raw_table) override;

  std::vector<Program> _programs;
};

template <size_t S>
void PatTable::Program::print(PmtTable::ConstStreamIt streams_begin,
                              PmtTable::ConstStreamIt streams_end,
                              Indent<S> indent, std::ostream &os) const {
  os << indent << "program:" << std::endl;
  os << indent + 1 << "number: " << static_cast<int>(_number) << std::endl;
  os << indent + 1 << "pmt_pid: 0x" << std::hex << static_cast<int>(_pid)
     << std::dec << std::endl;

  for (auto it = streams_begin; it != streams_end; ++it) {
    it->print(indent + 1, os);
  }
}

void handle_pat(Context &ctx, const RawTable &raw_table);

#endif
