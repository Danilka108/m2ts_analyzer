#ifndef M2TS_ANALYZER_PMT_PARSER_H
#define M2TS_ANALYZER_PMT_PARSER_H

#include "PsiParser.h"
#include "PsiSection.h"

struct Context;
struct Program;

class PmtParser final : public PsiParser {
public:
  PmtParser(Program *prg, uint16_t map_pid);

private:
  bool handle_section(PsiSection &psi_section) override;

  void reset_info() override;

  Program *prg;
};

#endif
