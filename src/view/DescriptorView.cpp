#include "DescriptorView.h"

#include "../common.h"

bool DescriptorView::is_subtitle() const {
  if (tag() != DID_TELETEXT && tag() != DID_VBI_TELETEXT) {
    return false;
  }

  const uint8_t *data = payload();
  size_t size = payload_size();

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
