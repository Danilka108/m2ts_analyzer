#include "PatTable.h"

#include "../Context.h"
#include "../common.h"
#include "../view/PatSectionView.h"
#include "../view/View.h"

struct Context;

bool PatTable::Program::deserialize(const ProgramView *program) {
  _number = program->prg_num();
  _pid = program->map_PID();

  return true;
}

bool PatTable::deserialize_payload(const RawTable &raw_table) {
  _programs.clear();

  for (auto it = raw_table.sections_begin(); it != raw_table.sections_end();
       ++it) {
    const PatSectionView *section =
        cast_view_to_view<const PatSectionView>(it->get());

    std::vector<std::shared_ptr<ProgramView>> prg_views;
    if (!parse_views(prg_views, section->payload_start(),
                     section->payload_size())) {
      _programs.clear();
      return false;
    }

    for (auto prg_view : prg_views) {
      Program program;
      if (!program.deserialize(prg_view.get())) {
        _programs.clear();
        return false;
      }

      if (program.number() == 0) {
        continue;
      }

      _programs.push_back(std::move(program));
    }
  }

  return true;
}

void handle_pat(Context &ctx, const RawTable &raw_table) {
  if (raw_table.is_applicable() == 0) {
    return;
  }

  if (!ctx.pat.deserialize(raw_table)) {
    return;
  }

  ctx.psi_demuxer.remove_handler(PAT_PID, PatTable::ID);
  ctx.pmts.clear();

  for (auto it = ctx.pat.programs_begin(); it != ctx.pat.programs_end(); ++it) {
    ctx.pmts.insert({it->number(), PmtTable()});
    ctx.psi_demuxer.add_handler(it->pid(), PatTable::ID, handle_pmt);
  }
}
