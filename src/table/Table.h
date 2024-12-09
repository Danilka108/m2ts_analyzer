#ifndef MPEGTS_ANALYZER_TABLE_H
#define MPEGTS_ANALYZER_TABLE_H

#include "../common.h"
#include "RawTable.h"

class Table {
 public:
  TID tid() const { return _tid; }

  TID_EXT tid_ext() const { return _tid_ext; }

  uint8_t version() const { return _version; }

  bool is_applicable() const { return _is_applicable; }

  bool is_deserialized() const { return _deserialized; }

  bool deserialize(const RawTable &raw_table);

  virtual ~Table() {}

 protected:
  virtual bool deserialize_payload(const RawTable &raw_table) = 0;

 private:
  TID _tid = 0;
  TID_EXT _tid_ext = 0;
  uint8_t _version = 0;
  bool _is_applicable = false;
  bool _deserialized = false;
};

#endif
