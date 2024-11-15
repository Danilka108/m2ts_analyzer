#ifndef M2TS_ANALYZER_CONTEXT_H
#define M2TS_ANALYZER_CONTEXT_H

#include "Program.h"
#include <vector>
struct Context {
public:
  std::vector<Program> programs;
  bool all_pat_found;
  bool all_programs_info_found;

  Context();
};

#endif
