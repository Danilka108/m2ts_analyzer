#include "PatParser.h"
#include "Context.h"
#include "PatSection.h"
#include "PsiParser.h"
#include "PsiSection.h"

PatParser::PatParser(Context &ctx) : PsiParser(PAT_PID), ctx(ctx) {}

bool PatParser::handle_section(PsiSection &psi_section) {
  PatSection pat_section(psi_section);
  if (!pat_section.is_valid()) {
    return false;
  }

  pat_section.extract_info(ctx);
  return true;
}

void PatParser::reset_info() { ctx.programs.clear(); }
