#include "ServiceDescription.h"
#include "Context.h"
#include "Descriptor.h"
#include "common.h"

ServiceDescription::ServiceDescription(uint8_t *buf)
    : service_id(GET_SHORT(buf)),
      descriptors_loop_length(GET_SHORT(buf + 3) & MASK(12)),
      full_length(5 + descriptors_loop_length), data(buf + 5) {}

void ServiceDescription::extract_info(Context &ctx) {
  uint8_t *info_start = data;
  uint8_t *info_end = data + descriptors_loop_length;

  while (info_start < info_end) {
    Descriptor desc(info_start);

    if (desc.tag == ServiceDescriptor::TAG) {
      ServiceDescriptor s_desc(desc);
      s_desc.extract_info(service_id, ctx);
    }

    if (desc.tag == ComponentDescriptor::TAG) {
      ComponentDescriptor c_desc(desc);
      c_desc.extract_info(ctx);
    }

    info_start += desc.full_length;
  }
}
