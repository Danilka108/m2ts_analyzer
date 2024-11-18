#ifndef M2TS_ANALYZER_SERVICE_DESCRIPTION_H
#define M2TS_ANALYZER_SERVICE_DESCRIPTION_H

#include <cstddef>
#include <cstdint>

struct Context;

struct ServiceDescription {
public:
  ServiceDescription(uint8_t *buf);

  uint16_t service_id;
  uint16_t descriptors_loop_length;

  size_t full_length;
  uint8_t *data;

  void extract_info(Context &ctx);
};

#endif
