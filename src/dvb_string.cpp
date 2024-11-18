#include "dvb_string.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iconv.h>

/* read and allocate a DVB string preceded by its length */
char *convert_dvb_string(const uint8_t *p, const uint8_t *p_end) {
  if (p == NULL) {
    return NULL;
  }

  int len;
  char *str;

  if (p_end - p < 1) {
    return NULL;
  }

  len = p[0];
  p++;
  if (len > p_end - p)
    return NULL;

  /*#if CONFIG_ICONV*/
  if (len) {
    const char *encodings[] = {"ISO6937",
                               "ISO-8859-5",
                               "ISO-8859-6",
                               "ISO-8859-7",
                               "ISO-8859-8",
                               "ISO-8859-9",
                               "ISO-8859-10",
                               "ISO-8859-11",
                               "",
                               "ISO-8859-13",
                               "ISO-8859-14",
                               "ISO-8859-15",
                               "",
                               "",
                               "",
                               "",
                               "",
                               "UCS-2BE",
                               "KSC_5601",
                               "GB2312",
                               "UCS-2BE",
                               "UTF-8",
                               "",
                               "",
                               "",
                               "",
                               "",
                               "",
                               "",
                               "",
                               "",
                               ""};
    iconv_t cd;
    char *in, *out;
    size_t inlen = len, outlen = inlen * 6 + 1;

    if (len >= 3 && p[0] == 0x10 && !p[1] && p[2] && p[2] <= 0xf &&
        p[2] != 0xc) {
      char iso8859[12];
      snprintf(iso8859, sizeof(iso8859), "ISO-8859-%d", p[2]);
      inlen -= 3;
      in = (char *)p + 3;
      cd = iconv_open("UTF-8", iso8859);
    } else if (p[0] < 0x20) {
      inlen -= 1;
      in = (char *)p + 1;
      cd = iconv_open("UTF-8", encodings[*p]);
    } else {
      in = (char *)p;
      cd = iconv_open("UTF-8", encodings[0]);
    }
    if (cd == (iconv_t)-1)
      goto no_iconv;
    str = out = (char *)malloc(outlen);
    if (!str) {
      iconv_close(cd);
      return NULL;
    }
    if (iconv(cd, &in, &inlen, &out, &outlen) == -1) {
      iconv_close(cd);
      free(str);
      goto no_iconv;
    }
    iconv_close(cd);
    *out = 0;
    /**pp = p + len;*/
    return str;
  }
no_iconv:
  /*#endif*/
  str = (char *)malloc(len + 1);
  if (!str)
    return NULL;
  memcpy(str, p, len);
  str[len] = '\0';
  p += len;
  /**pp = p;*/
  return str;
}
