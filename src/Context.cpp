#include "Context.h"
#include "ElementaryStream.h"
#include "PmtParser.h"
#include <cstdlib>

Context::Context() {}

Program::Program(uint16_t id, uint16_t pmt_pid)
    : id(id), description(nullptr), provider_name(nullptr), name(nullptr),
      streams(), pmt_parser(this, pmt_pid) {}

Program::~Program() {
  if (provider_name != nullptr) {
    free(provider_name);
  }

  if (name != nullptr) {
    free(name);
  }
}

Stream::Stream(ElementaryStream &es)
    : type(es.stream_type), stream_class(STREAM_UNKNOWN_CLASS),
      pid(es.elementary_pid), component_tag(-1) {
  es.extract_info(*this);
}

Component::Component(const char *description, char *second_description,
                     char *lang_code, uint8_t component_tag)
    : description(description), second_description(second_description),
      lang_code(lang_code), component_tag(component_tag) {}

Component::~Component() {}
