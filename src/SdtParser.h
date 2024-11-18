#ifndef M2TS_ANALYZER_SDT_PARSER_H
#define M2TS_ANALYZER_SDT_PARSER_H

#include "PsiParser.h"
#include "PsiSection.h"

class SdtParser : public PsiParser {
public:
  SdtParser(Context &ctx);

  static constexpr uint16_t SDT_PID = 0x0011;
private:
  bool handle_section(PsiSection &psi_section) override;

  void reset_info() override;


  Context &ctx;
};

#endif
