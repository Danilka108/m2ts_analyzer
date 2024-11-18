#include "ElementaryStream.h"
#include "Context.h"
#include "Descriptor.h"
#include "common.h"

ElementaryStream::ElementaryStream(uint8_t *buf)
    : stream_type(buf[0]), elementary_pid(GET_SHORT(buf + 1) & MASK(13)),
      es_info_length(GET_SHORT(buf + 3) & MASK(12)),
      full_length(5 + es_info_length), data(buf + 5) {}

void ElementaryStream::extract_info(Stream &stream) {
  uint8_t *info_start = data;
  uint8_t *info_end = data + es_info_length;
  /** TODO
   * bool atsc = standarts & standarts::ATSC;
   */
  bool atsc = false;
  bool is_subtitle = false;

  while (info_start < info_end) {
    Descriptor desc(info_start);

    if (desc.tag == StreamIdentifierDescriptor::TAG) {
      StreamIdentifierDescriptor si_desc(desc);
      si_desc.extract_info(stream);
    }

    if (desc.tag == SubtitlingDescriptor::TAG ||
        atsc && desc.tag == AtscCaptionDescriptor::TAG) {
      is_subtitle = true;
    }

    if (desc.tag == TeletextDescritpor::TAG) {
      TeletextDescritpor td(desc);
      is_subtitle = is_subtitle || td.is_subtitle();
    }

    if (desc.tag == VbiTeletextDescritpor::TAG) {
      VbiTeletextDescritpor td(desc);
      is_subtitle = is_subtitle || td.is_subtitle();
    }

    info_start += desc.full_length;
  }

  if (is_subtitle) {
    stream.stream_class = STREAM_SUBTITLE_CLASS;
  }

  if (stream_type_is_video(stream_type)) {
    stream.stream_class = STREAM_VIDEO_CLASS;
  }

  if (stream_type_is_audio(stream_type)) {
    stream.stream_class = STREAM_AUDIO_CLASS;
  }
}
