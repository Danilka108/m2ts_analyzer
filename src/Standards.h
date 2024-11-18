#ifndef M2TS_ANALYZER_STANDARDS_H
#define M2TS_ANALYZER_STANDARDS_H

#include <cstdint>

enum class Standards : uint16_t {
  /**
   * No known standard
   */
  NONE = 0x0000,
  /**
   * Defined by MPEG, common to all standards
   */
  MPEG = 0x0001, //!<
  /**
   * Defined by ETSI/DVB
   */
  DVB = 0x0002,
  /**
   * Defined by ANSI/SCTE
   */
  SCTE = 0x0004,
  /**
   * Defined by ATSC
   */
  ATSC = 0x0008,
  /**
   * Defined by ISDB
   */
  ISDB = 0x0010,
  /**
   * Defined in Japan only (typically in addition to ISDB)
   */
  JAPAN = 0x0020,
  /**
   * Defined by ABNT (Brazil, typically in addition to ISDB)
   */
  ABNT = 0x0040,
};

#endif
