#ifndef M2TS_ANALYZER_PAT_PARSER_H
#define M2TS_ANALYZER_PAT_PARSER_H

#include "PsiParser.h"

struct Context;

class PatParser final : public PsiParser {
public:
  PatParser(Context &ctx);

private:
  bool handle_section(PsiSection &psi_section) override;

  void reset_info() override;

  static constexpr uint16_t PAT_PID = 0;

  Context &ctx;
};

#endif
