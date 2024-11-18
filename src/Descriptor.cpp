#include "Descriptor.h"
#include "Context.h"
#include "common.h"
#include "dvb_string.h"
#include <cstdlib>
#include <cstring>

const char *get_stream_description(uint8_t stream_content,
                                   uint8_t component_type);
const char *get_service_description(unsigned char service_type);

Descriptor::Descriptor(uint8_t *buf)
    : tag(buf[0]), length(buf[1]), full_length(2 + length), data(buf + 2) {}

ServiceDescriptor::ServiceDescriptor(Descriptor &descriptor)
    : Descriptor(descriptor) {}

void ServiceDescriptor::extract_info(uint16_t service_id, Context &ctx) {
  uint8_t *s_start = data;
  uint8_t type = 0;

  uint8_t *s_prv_name_start = NULL;
  uint8_t s_prv_name_length = 0;
  char *s_prv_name = NULL;

  uint8_t *s_name_start = NULL;
  uint8_t s_name_length = 0;
  char *s_name = NULL;

  size_t i = 0;

  while (s_start < data + length) {
    type = s_start[0];

    s_prv_name_start = s_start + 1;
    s_name_start = s_prv_name_start + 1 + s_prv_name_start[0];
    s_start = s_name_start + 1 + s_name_start[0];

    s_prv_name = convert_dvb_string((const uint8_t *)(s_prv_name_start),
                                    (const uint8_t *)(data + length));

    s_name = convert_dvb_string((const uint8_t *)(s_name_start),
                                (const uint8_t *)(data + length));

    for (auto &program : ctx.programs) {
      if (program.id == service_id) {
        program.description = get_service_description(type);
        program.name = s_name;
        program.provider_name = s_prv_name;
        break;
      }
    }

    /*s_start = s_name_start + 1 + s_name_length;*/
  }
}

StreamIdentifierDescriptor::StreamIdentifierDescriptor(Descriptor &descriptor)
    : Descriptor(descriptor), component_tag(data[0]) {}

void StreamIdentifierDescriptor::extract_info(Stream &stream) {
  stream.component_tag = component_tag;
}

ComponentDescriptor::ComponentDescriptor(Descriptor &descriptor)
    : Descriptor(descriptor), stream_content(data[0] & MASK(4)),
      component_type(data[1]), component_tag(data[2]),
      ISO_639_language_code{data[3], data[4], data[5], '\0'} {}

void ComponentDescriptor::extract_info(Context &ctx) {
  const char *description =
      get_stream_description(stream_content, component_type);
  uint8_t *text_start = data + 6;
  char *second_description = convert_dvb_string(
      (const uint8_t *)text_start, (const uint8_t *)(data + length));
  char *lang_code = (char *)malloc(sizeof(ISO_639_language_code));
  memcpy(lang_code, ISO_639_language_code, sizeof(ISO_639_language_code));

  ctx.components.emplace_front(description, second_description, lang_code,
                               component_tag);
}

SubtitlingDescriptor::SubtitlingDescriptor(Descriptor &descriptor)
    : Descriptor(descriptor) {}

AtscCaptionDescriptor::AtscCaptionDescriptor(Descriptor &descriptor)
    : Descriptor(descriptor) {}

TeletextDescritpor::TeletextDescritpor(Descriptor &descriptor)
    : Descriptor(descriptor) {}

bool is_desc_subtitle(Descriptor &desc) {
  uint8_t *data = desc.data;
  size_t size = desc.length;

  while (size >= 5) {
    uint8_t ttype = data[3] >> 3;
    if (ttype == 0x02 || ttype == 0x05) {
      return true;
    }

    size -= 5;
    data += 5;
  }

  return false;
}

bool TeletextDescritpor::is_subtitle() { return is_desc_subtitle(*this); }

VbiTeletextDescritpor::VbiTeletextDescritpor(Descriptor &descriptor)
    : Descriptor(descriptor) {}

bool VbiTeletextDescritpor::is_subtitle() { return is_desc_subtitle(*this); }

const char *get_stream_description(uint8_t stream_content,
                                   uint8_t component_type) {
  switch (stream_content) {
  case 0x01:
    switch (component_type) {
    case 0x01:
      return "MPEG-2 video, 4:3 aspect ratio, 25 Hz";
    case 0x02:
      return "MPEG-2 video, 16:9 aspect ratio with pan vectors, 25 Hz";
    case 0x03:
      return "MPEG-2 video, 16:9 aspect ratio without pan vectors, 25 Hz";
    case 0x04:
      return "MPEG-2 video, > 16:9 aspect ratio, 25 Hz";
    case 0x05:
      return "MPEG-2 video, 4:3 aspect ratio, 30 Hz";
    case 0x06:
      return "MPEG-2 video, 16:9 aspect ratio with pan vectors, 30 Hz";
    case 0x07:
      return "MPEG-2 video, 16:9 aspect ratio without pan vectors, 30 Hz";
    case 0x08:
      return "MPEG-2 video, > 16:9 aspect ratio, 30 Hz";
    case 0x09:
      return "MPEG-2 high definition video, 4:3 aspect ratio, 25 Hz";
    case 0x0A:
      return "MPEG-2 high definition video, 16:9 aspect ratio with pan "
             "vectors, 25 Hz";
    case 0x0B:
      return "MPEG-2 high definition video, 16:9 aspect ratio without pan "
             "vectors, 25 Hz";
    case 0x0C:
      return "MPEG-2 high definition video, > 16:9 aspect ratio, 25 Hz";
    case 0x0D:
      return "MPEG-2 high definition video, 4:3 aspect ratio, 30 Hz";
    case 0x0E:
      return "MPEG-2 high definition video, 16:9 aspect ratio with pan "
             "vectors, 30 Hz";
    case 0x0F:
      return "MPEG-2 high definition video, 16:9 aspect ratio without pan "
             "vectors, 30 Hz";
    case 0x10:
      return "MPEG-2 high definition video, > 16:9 aspect ratio, 30 Hz";
    case 0xB0:
      return "";
    case 0xFF:
      return "";
    default:
      return "";
    }

  case 0x02:
    switch (component_type) {
    case 0x01:
      return "MPEG-1 Layer 2 audio, single mono channel";
    case 0x02:
      return "MPEG-1 Layer 2 audio, dual mono channel";
    case 0x03:
      return "MPEG-1 Layer 2 audio, stereo (2 channel)";
    case 0x04:
      return "MPEG-1 Layer 2 audio, multi-lingual, multi-channel";
    case 0x05:
      return "MPEG-1 Layer 2 audio, surround sound";
    case 0x40:
      return "MPEG-1 Layer 2 audio description for the visually impaired";
    case 0x41:
      return "MPEG-1 Layer 2 audio for the hard of hearing";
    case 0x42:
      return "receiver-mixed supplementary audio as per annex E of TS 101 154";
    case 0x47:
      return "MPEG-1 Layer 2 audio, receiver mix audio description";
    case 0x48:
      return "MPEG-1 Layer 2 audio, broadcaster mix audio description";
    case 0xB0:
      return "";
    case 0xFF:
      return "";
    default:
      return "";
    }

  case 0x03:
    switch (component_type) {
    case 0x01:
      return "EBU Teletext subtitles";
    case 0x02:
      return "associated EBU Teletext";
    case 0x10:
      return "DVB subtitles (normal) with no monitor aspect ratio criticality";
    case 0x11:
      return "DVB subtitles (normal) for display on 4:3 aspect ratio monitor";
    case 0x12:
      return "DVB subtitles (normal) for display on 16:9 aspect ratio monitor";
    case 0x13:
      return "DVB subtitles (normal) for display on 2.21:1 aspect ratio "
             "monitor";
    case 0x14:
      return "DVB subtitles (normal) for display on a high definition monitor";
    case 0x20:
      return "DVB subtitles (for the hard of hearing) with no monitor aspect "
             "ratio criticality";
    case 0x21:
      return "DVB subtitles (for the hard of hearing) for display on 4:3 "
             "aspect ratio monitor";
    case 0x22:
      return "DVB subtitles (for the hard of hearing) for display on 16:9 "
             "aspect ratio monitor";
    case 0x30:
      return "Open (in-vision) sign language interpretation for the deaf";
    case 0x31:
      return "Closed sign language interpretation for the deaf";
    case 0x40:
      return "video up-sampled from standard definition source material";
    case 0xB0:
      return "";
    case 0xFF:
      return "";
    default:
      return "";
    }

  case 0x05:
    switch (component_type) {
    case 0x01:
      return "H.264/AVC standard definition video, 4:3 aspect ratio, 25 Hz";
    case 0x03:
      return "H.264/AVC standard definition video, 16:9 aspect ratio, 25 Hz";
    case 0x04:
      return "H.264/AVC standard definition video, > 16:9 aspect ratio, 25 Hz";
    case 0x05:
      return "H.264/AVC standard definition video, 4:3 aspect ratio, 30 Hz";
    case 0x07:
      return "H.264/AVC standard definition video, 16:9 aspect ratio, 30 Hz";
    case 0x0B:
      return "H.264/AVC high definition video, 16:9 aspect ratio, 25 Hz";
    case 0x0C:
      return "H.264/AVC high definition video, > 16:9 aspect ratio, 25 Hz";
    case 0x0F:
      return "H.264/AVC high definition video, 16:9 aspect ratio, 30 Hz";
    case 0x80:
      return "H.264/AVC plano-stereoscopic frame compatible high definition "
             "video, 16:9 aspect ratio, 25 Hz, Side-by-Side";
    case 0x81:
      return "H.264/AVC plano-stereoscopic frame compatible high definition "
             "video, 16:9 aspect ratio, 25 Hz, Top-and-Bottom";
    case 0xB0:
      return "";
    case 0xFF:
      return "";
    default:
      return "";
    }

  case 0x06:
    switch (component_type) {
    case 0x01:
      return "HE-AAC audio, single mono channel";
    case 0x03:
      return "HE-AAC audio, stereo";
    case 0x05:
      return "HE-AAC audio, surround sound";
    case 0x40:
      return "HE-AAC audio description for the visually impaired";
    case 0x41:
      return "HE-AAC audio for the hard of hearing";
    case 0x42:
      return "HE-AAC receiver-mixed supplementary audio as per annex E of TS "
             "101 154";
    case 0x43:
      return "HE-AAC v2 audio, stereo";
    case 0x47:
      return "HE-AAC receiver mix audio description for the visually impaired";
    case 0x48:
      return "HE-AAC broadcaster mix audio description for the visually "
             "impaired";
    case 0xB0:
      return "";
    case 0xFF:
      return "";
    default:
      return "";
    }

  case 0x07: // DTS Audio
    switch (component_type) {
    case 0x00:
      return "";
    case 0x80:
      return "reserved for future use";
    case 0xFF:
      return "reserved for future use";
    default:
      return "";
    }

  case 0x08:
    switch (component_type) {
    case 0x01:
      return "DVB SRM data";
    case 0x02:
      return "reserved for DVB CPCM modes";
    case 0xFF:
      return "reserved for future use";
    default:
      return "";
    }

  default:
    return "";
  }
}

const char *get_service_description(uint8_t service_type) {
  switch (service_type) {
  case 0x00:
    return "";
  case 0x01:
    return "digital television service";
  case 0x02:
    return "digital radio sound service";
  case 0x03:
    return "Teletext service";
  case 0x04:
    return "NVOD reference service";
  case 0x05:
    return "NVOD time-shifted service";
  case 0x06:
    return "mosaic service";
  case 0x07:
    return "FM radio service";
  case 0x08:
    return "DVB SRM service";
  case 0x09:
    return "";
  case 0x0A:
    return "advanced codec digital radio sound service";
  case 0x0B:
    return "advanced codec mosaic service";
  case 0x0C:
    return "data broadcast service";
  case 0x0D:
    return "";
  case 0x0E:
    return "RCS Map";
  case 0x0F:
    return "RCS FLS";
  case 0x10:
    return "DVB MHP service";
  case 0x11:
    return "MPEG-2 HD digital television service";
  case 0x12:
  case 0x13:
  case 0x14:
  case 0x15:
    return "";
  case 0x16:
    return "advanced codec SD digital television service";
  case 0x17:
    return "advanced codec SD NVOD time-shifted service";
  case 0x18:
    return "advanced codec SD NVOD reference service";
  case 0x19:
    return "advanced codec HD digital television service";
  case 0x1A:
    return "advanced codec HD NVOD time-shifted service";
  case 0x1B:
    return "advanced codec HD NVOD reference service";
  case 0x1C:
    return "advanced codec frame compatible plano-stereoscopic HD digital "
           "television service";
  case 0x1D:
    return "advanced codec frame compatible plano-stereoscopic HD NVOD "
           "time-shifted service";
  case 0x1E:
    return "advanced codec frame compatible plano-stereoscopic HD NVOD "
           "reference service";
  case 0x1F ... 0x7F:
    return "";
  case 0x80 ... 0xFE:
    return "";
  case 0xFF:
    return "";
  default:
    return "";
  }
}
