#include "Table.h"

bool Table::deserialize(const RawTable &raw_table) {
  _deserialized = false;

  if (raw_table.sections_begin() == raw_table.sections_end()) {
    return false;
  }

  const SectionView *section = raw_table.sections_begin()->get();
  _tid = section->tid();
  _tid_ext = section->tid_ext();
  _version = section->version_number();
  _is_applicable = section->CNI();

  _deserialized = deserialize_payload(raw_table);
  return _deserialized;
}
