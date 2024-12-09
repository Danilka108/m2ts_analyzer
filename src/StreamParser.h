#ifndef MPEGTS_ANALYZER_TS_PARSER_H
#define MPEGTS_ANALYZER_TS_PARSER_H

#include "Context.h"
#include "stream/Stream.h"

struct StreamParser {
public:
  StreamParser(std::unique_ptr<Stream> stream)
      : ctx(), stream(std::move(stream)) {}

  void parse();

  template <size_t S> void print_programs(Indent<S> indent, std::ostream &os);

private:
  Context ctx;
  std::unique_ptr<Stream> stream;
};

template <size_t S>
void StreamParser::print_programs(Indent<S> indent, std::ostream &os) {
  for (auto it = ctx.pat.programs_begin(); it != ctx.pat.programs_end(); ++it) {
    auto found_pmt = ctx.pmts.find(it->number());
    if (found_pmt == ctx.pmts.end()) {
      continue;
    }

    PmtTable &pmt = found_pmt->second;
    it->print(pmt.streams_begin(), pmt.streams_end(), indent, os);
  }
}

#endif
