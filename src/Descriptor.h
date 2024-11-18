#ifndef M2TS_ANALYZER_DESCRIPTOR_H
#define M2TS_ANALYZER_DESCRIPTOR_H

#include "PSI.h"
#include <cstddef>
#include <cstdint>

struct Context;
struct Program;
struct Stream;

struct Descriptor {
public:
  Descriptor(uint8_t *buf);

  uint8_t tag;
  uint8_t length;

  size_t full_length;
  uint8_t *data;
};

struct ServiceDescriptor : Descriptor {
public:
  ServiceDescriptor(Descriptor &descriptor);

  void extract_info(uint16_t service_id, Context &ctx);

  static constexpr uint8_t TAG = 0x48;
};

const char *get_service_description(unsigned char service_type);

struct StreamIdentifierDescriptor : Descriptor {
public:
  StreamIdentifierDescriptor(Descriptor &descriptor);

  void extract_info(Stream &stream);

  uint8_t component_tag;

  static constexpr uint8_t TAG = 0x52;
};

struct ComponentDescriptor : Descriptor {
public:
  ComponentDescriptor(Descriptor &descriptor);

  void extract_info(Context &ctx);

  uint8_t stream_content;
  uint8_t component_type;
  uint8_t component_tag;
  uint8_t ISO_639_language_code[4];

  static constexpr uint8_t TAG = 0x50;
};

struct SubtitlingDescriptor : Descriptor {
public:
  SubtitlingDescriptor(Descriptor &descriptor);

  static constexpr uint8_t TAG = DID_SUBTITLING;
};

struct AtscCaptionDescriptor : Descriptor {
public:
  AtscCaptionDescriptor(Descriptor &descriptor);

  static constexpr uint8_t TAG = DID_ATSC_CAPTION;
};

struct TeletextDescritpor : Descriptor {
public:
  TeletextDescritpor(Descriptor &descriptor);

  bool is_subtitle();

  static constexpr uint8_t TAG = DID_TELETEXT;
};

struct VbiTeletextDescritpor : Descriptor {
public:
  VbiTeletextDescritpor(Descriptor &descriptor);

  bool is_subtitle();

  static constexpr uint8_t TAG = DID_VBI_TELETEXT;
};

#endif
