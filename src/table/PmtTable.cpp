#include "PmtTable.h"

#include "../Context.h"
#include "../view/DescriptorView.h"
#include "../view/PmtSectionView.h"
#include "../view/View.h"
#include "RawTable.h"

bool PmtTable::deserialize_payload(const RawTable &raw_table) {
  _descriptors.clear();
  _streams.clear();

  for (auto it = raw_table.sections_begin(); it != raw_table.sections_end();
       ++it) {
    const PmtSectionView *section =
        cast_view_to_view<const PmtSectionView>(it->get());

    if (!parse_views<DescriptorView>(_descriptors, section->info_start(),
                                     section->info_size())) {
      _descriptors.clear();
      return false;
    }

    std::vector<std::shared_ptr<EsView>> elem_streams;
    if (!parse_views<EsView>(elem_streams, section->payload_start(),
                             section->payload_size())) {
      _descriptors.clear();
      return false;
    }

    for (auto &es : elem_streams) {
      Stream stream;
      if (!stream.deserialize(es.get())) {
        _descriptors.clear();
        _streams.clear();
        return false;
      }

      _streams.push_back(std::move(stream));
    }
  }

  return true;
}

bool PmtTable::Stream::deserialize(const EsView *es) {
  _descriptors.clear();
  if (!parse_views<DescriptorView>(_descriptors, es->info_start(),
                                   es->info_size())) {
    return false;
  }

  _type = es->stream_type();
  _pid = es->pid();

  bool is_subtitle = false;
  for (auto descriptor : _descriptors) {
    if (descriptor->is_subtitle()) {
      is_subtitle = true;
      break;
    }
  }

  if (is_subtitle) {
    _stream_class = SUBTITLE_CLASS;
  }

  if (is_video()) {
    _stream_class = VIDEO_CLASS;
  }

  if (is_audio()) {
    _stream_class = AUDIO_CLASS;
  }

  return true;
}

void handle_pmt(Context &ctx, const RawTable &raw_table) {
  auto found_pmt = ctx.pmts.find(raw_table.tid_ext());
  auto &pmt =
      found_pmt == ctx.pmts.end()
          ? ctx.pmts.insert({raw_table.tid_ext(), PmtTable()}).first->second
          : found_pmt->second;

  pmt.deserialize(raw_table);
}

bool PmtTable::Stream::is_video() {
  return _type == ST_MPEG1_VIDEO || _type == ST_MPEG2_VIDEO ||
         _type == ST_MPEG4_VIDEO || _type == ST_AUX_VIDEO ||
         _type == ST_J2K_VIDEO || _type == ST_MPEG2_3D_VIEW || is_avc() ||
         is_hevc() || is_vvc() || _type == ST_JPEG_XS_VIDEO ||
         _type == ST_EVC_VIDEO || _type == ST_LCEVC_VIDEO ||
         _type == ST_AVS3_VIDEO;
}

bool PmtTable::Stream::is_avc() {
  return _type == ST_AVC_VIDEO || _type == ST_AVC_SUBVIDEO_G ||
         _type == ST_AVC_SUBVIDEO_H || _type == ST_AVC_SUBVIDEO_I ||
         _type == ST_AVC_3D_VIEW;
}

bool PmtTable::Stream::is_hevc() {
  return _type == ST_HEVC_VIDEO || _type == ST_HEVC_SUBVIDEO ||
         _type == ST_HEVC_SUBVIDEO_G || _type == ST_HEVC_SUBVIDEO_TG ||
         _type == ST_HEVC_SUBVIDEO_H || _type == ST_HEVC_SUBVIDEO_TH ||
         _type == ST_HEVC_TILESET;
}

bool PmtTable::Stream::is_vvc() {
  return _type == ST_VVC_VIDEO || _type == ST_VVC_VIDEO_SUBSET;
}

bool PmtTable::Stream::is_audio() {
  /*
  bool audio = false;
  if (regid == REGID_HDMV) {
      audio = _type == ST_LPCM_AUDIO       ||
              _type == ST_HDMV_AC3         ||
              _type == ST_DTS_AUDIO        ||
              _type == ST_HDMV_AC3_TRUEHD  ||
              _type == ST_HDMV_AC3_PLUS    ||
              _type == ST_DTS_HS_AUDIO     ||
              _type == ST_DTS_HD_MA_AUDIO  ||
              _type == ST_HDMV_EAC3        ||
              _type == ST_DTS_AUDIO_8A     ||
              _type == ST_SDDS_AUDIO       ||
              _type == ST_HDMV_AC3_PLS_SEC ||
              _type == ST_DTS_HD_SEC;
  }
  */
  return /*audio                     ||*/
      _type == ST_MPEG1_AUDIO || _type == ST_MPEG2_AUDIO ||
      _type == ST_MPEG4_AUDIO || _type == ST_AAC_AUDIO ||
      _type == ST_AC3_AUDIO || _type == ST_AC3_PLUS_AUDIO ||
      _type == ST_AC3_TRUEHD_AUDIO || _type == ST_A52B_AC3_AUDIO ||
      _type == ST_EAC3_AUDIO || _type == ST_MPEG4_AUDIO_RAW ||
      _type == ST_MPH3D_MAIN || _type == ST_MPH3D_AUX ||
      _type == ST_AVS2_AUDIO || _type == ST_AVS3_AUDIO;
}

const char *PmtTable::Stream::stream_class_to_desc() const {
  switch (_stream_class) {
  case PmtTable::Stream::AUDIO_CLASS:
    return "audio";
  case PmtTable::Stream::VIDEO_CLASS:
    return "video";
  case PmtTable::Stream::SUBTITLE_CLASS:
    return "subtitle";
  default:
    return "unknown";
    break;
  }
}
