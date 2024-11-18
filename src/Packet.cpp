#include "Packet.h"
#include "BitStream.h"

    /*: buf(buf), sync_byte(*buf.data),*/
    /*  transport_error_indicator(GET_BIT(buf.data + 1, 7)),*/
    /*  payload_until_start_indicator(GET_BIT(buf.data + 1, 6)),*/
    /*  pid(GET_SHORT(buf.data + 1) & MASK(13)),*/
    /*  adaptation_field_control(GET_BITS(buf.data + 3, 4, 6)),*/
    /*  continuity_counter(GET_BITS(buf.data + 3, 0, 4)),*/
    /*  data(calc_data_ptr(buf, adaptation_field_control)),*/
    /*  end(buf.data + TS_PACKET_SIZE), data_len(end - data) {}*/
