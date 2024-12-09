#ifndef MPEGTS_ANALYZER_CONTEXT_H
#define MPEGTS_ANALYZER_CONTEXT_H

#include <unordered_map>

#include "PsiDemuxer.h"
#include "common.h"
#include "table/PatTable.h"
#include "table/PmtTable.h"

struct Context {
 public:
  Context()
      : pat(),
        pmts {},
        psi_demuxer(*this) {
    psi_demuxer.add_handler(PAT_PID, PatTable::ID, handle_pat);
  }

  Context(const Context &) = delete;
  Context(Context &&) = delete;

  Context &operator=(const Context &) = delete;
  Context &operator=(Context &&) = delete;

  PatTable pat;
  std::unordered_map<PRG_NUM, PmtTable> pmts;
  PsiDemuxer psi_demuxer;
};

#endif
