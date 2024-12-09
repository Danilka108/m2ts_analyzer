#include "RawTable.h"

#include <cstring>

#include "../view/View.h"

bool RawTable::add_section(const SectionView *section) {
  if (!section) {
    return false;
  }

  if (section->SSI() != 1) {
    return false;
  }

  if (section->section_number() > section->last_section_number()) {
    return false;
  }

  if (_sections.size() == 0) {
    _tid = section->tid();
    _tid_ext = section->tid_ext();
    _version = section->version_number();
    _curr_next_indicator = section->CNI();
    _sections_nb = 0;

    _sections.clear();
    _sections.reserve(section->last_section_number() + 1);
    for (size_t i = 0; i < section->last_section_number() + 1; i++) {
      _sections.push_back(nullptr);
    }
  }

  if (section->version_number() != _version ||
      section->CNI() != _curr_next_indicator || section->tid() != _tid ||
      section->tid_ext() != _tid_ext ||
      section->last_section_number() + 1 != _sections.size()) {
    return false;
  }

  if (_sections[section->section_number()] != nullptr) {
    return false;
  }

  _sections_nb++;
  _sections[section->section_number()] =
      std::unique_ptr<SectionView>(copy_view(section));

  return true;
}
