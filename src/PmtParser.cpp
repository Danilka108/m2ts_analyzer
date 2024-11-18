#include "PmtParser.h"
#include "Context.h"
#include "PmtSection.h"

PmtParser::PmtParser(Program *prg, uint16_t map_pid)
    : PsiParser(map_pid), prg(prg) {}

bool PmtParser::handle_section(PsiSection &psi_section) {
  PmtSection pmt_section(psi_section);

  if (!pmt_section.is_valid()) {
    return false;
  }

  if (pmt_section.program_number != prg->id) {
    return false;
  }

  pmt_section.extract_info(prg);
  return true;
}

void PmtParser::reset_info() {}
