#ifndef M2TS_ANALYZER_CONTEXT_H
#define M2TS_ANALYZER_CONTEXT_H

#include "ElementaryStream.h"
#include "PmtParser.h"
#include <cstdint>
#include <forward_list>

struct Program;
struct Stream;
struct Component;

struct Context {
public:
  std::forward_list<Program> programs;
  std::forward_list<Component> components;

  Context();
};

struct Program {
public:
  Program(uint16_t id, uint16_t pmt_pid);

  uint16_t id;
  /*uint16_t pmt_pid;*/
  const char *description;
  char *provider_name;
  char *name;

  std::forward_list<Stream> streams;
  PmtParser pmt_parser;

  ~Program();
};

struct Stream {
public:
  Stream(ElementaryStream &es);

  uint8_t type;
  StreamClass stream_class;
  uint16_t pid;
  int16_t component_tag;
};

struct Component {
public:
  Component(const char *description, char *second_description, char *lang_code, uint8_t component_tag);

  const char *description;
  char *second_description;
  char *lang_code;
  uint8_t component_tag;

  ~Component();
};

#endif
