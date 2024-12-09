#ifndef MPEGTS_ANALYZER_INDENT_H
#define MPEGTS_ANALYZER_INDENT_H

#include <cstddef>
#include <ostream>

template <size_t S> struct Indent {
  size_t amount;
  static constexpr size_t SIZE = S;

  friend Indent<S> operator+(Indent<S> lhs, size_t additional_amount) {
    lhs.amount += additional_amount;
    return lhs;
  }
};

template <size_t S>
std::ostream &operator<<(std::ostream &os, const Indent<S> &indent) {
  for (size_t i = 0; i < indent.amount; i++) {
    for (size_t j = 0; j < S; j++) {
      os << " ";
    }
  }

  return os;
}

#endif
