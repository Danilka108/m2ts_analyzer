#ifndef MPEGTS_ANALYZER_RAW_TABLE_H
#define MPEGTS_ANALYZER_RAW_TABLE_H

#include <vector>

#include "../view/SectionView.h"

class RawTable {
 public:
  RawTable() {}

  RawTable(const RawTable &) = delete;
  RawTable(RawTable &&) = default;

  RawTable &operator=(const RawTable &) = delete;
  RawTable &operator=(RawTable &&) = default;

  bool is_applicable() const { return _curr_next_indicator; }

  int8_t version() const { return _version; }

  TID tid() const { return _tid; }

  TID_EXT tid_ext() const { return _tid_ext; }

  ConstSectionIt sections_begin() const { return _sections.begin(); }

  ConstSectionIt sections_end() const { return _sections.end(); }

  bool is_uninit() { return _sections.size() == 0; }

  bool is_collected() const {
    return _sections.size() != 0 && _sections_nb == _sections.size();
  }

  bool add_section(const SectionView *section);

 private:
  size_t _sections_nb = 0;
  int8_t _version = 0;
  uint8_t _curr_next_indicator = 0;
  TID _tid = 0;
  TID_EXT _tid_ext = 0;

  std::vector<std::unique_ptr<SectionView>> _sections = {};
};

#endif
