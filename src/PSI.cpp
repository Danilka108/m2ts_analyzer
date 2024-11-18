#include "PSI.h"

bool stream_type_is_video(uint8_t st) {
  return st == ST_MPEG1_VIDEO || st == ST_MPEG2_VIDEO || st == ST_MPEG4_VIDEO ||
         st == ST_AUX_VIDEO || st == ST_J2K_VIDEO || st == ST_MPEG2_3D_VIEW ||
         stream_type_is_avc(st) || stream_type_is_hevc(st) || stream_type_is_vvc(st) ||
         st == ST_JPEG_XS_VIDEO || st == ST_EVC_VIDEO || st == ST_LCEVC_VIDEO ||
         st == ST_AVS3_VIDEO;
}

bool stream_type_is_avc(uint8_t st) {
  return st == ST_AVC_VIDEO || st == ST_AVC_SUBVIDEO_G ||
         st == ST_AVC_SUBVIDEO_H || st == ST_AVC_SUBVIDEO_I ||
         st == ST_AVC_3D_VIEW;
}

bool stream_type_is_hevc(uint8_t st) {
  return st == ST_HEVC_VIDEO || st == ST_HEVC_SUBVIDEO ||
         st == ST_HEVC_SUBVIDEO_G || st == ST_HEVC_SUBVIDEO_TG ||
         st == ST_HEVC_SUBVIDEO_H || st == ST_HEVC_SUBVIDEO_TH ||
         st == ST_HEVC_TILESET;
}

bool stream_type_is_vvc(uint8_t st) {
  return st == ST_VVC_VIDEO || st == ST_VVC_VIDEO_SUBSET;
}

bool stream_type_is_audio(uint8_t st) {
  /*
  bool audio = false;
  if (regid == REGID_HDMV) {
      audio = st == ST_LPCM_AUDIO       ||
              st == ST_HDMV_AC3         ||
              st == ST_DTS_AUDIO        ||
              st == ST_HDMV_AC3_TRUEHD  ||
              st == ST_HDMV_AC3_PLUS    ||
              st == ST_DTS_HS_AUDIO     ||
              st == ST_DTS_HD_MA_AUDIO  ||
              st == ST_HDMV_EAC3        ||
              st == ST_DTS_AUDIO_8A     ||
              st == ST_SDDS_AUDIO       ||
              st == ST_HDMV_AC3_PLS_SEC ||
              st == ST_DTS_HD_SEC;
  }
  */
  return /*audio                     ||*/
      st == ST_MPEG1_AUDIO || st == ST_MPEG2_AUDIO || st == ST_MPEG4_AUDIO ||
      st == ST_AAC_AUDIO || st == ST_AC3_AUDIO || st == ST_AC3_PLUS_AUDIO ||
      st == ST_AC3_TRUEHD_AUDIO || st == ST_A52B_AC3_AUDIO ||
      st == ST_EAC3_AUDIO || st == ST_MPEG4_AUDIO_RAW || st == ST_MPH3D_MAIN ||
      st == ST_MPH3D_AUX || st == ST_AVS2_AUDIO || st == ST_AVS3_AUDIO;
}
