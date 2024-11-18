#include "SdtParser.h"
#include "Context.h"
#include "SdtSection.h"

SdtParser::SdtParser(Context &ctx) : PsiParser(SDT_PID), ctx(ctx) {}

bool SdtParser::handle_section(PsiSection &psi_section) {
  SdtSection sdt_section(psi_section);

  if (!sdt_section.is_valid()) {
    return false;
  }

  sdt_section.extract_info(ctx);
  return true;
}

void SdtParser::reset_info() {}
