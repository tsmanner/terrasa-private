#pragma once

#include <functional>

#include "Show.h"

namespace lazy {

template <typename T>
struct Precedence;

template <>
struct Precedence<int> {
  static constexpr int value = 0;
};

template <>
struct Precedence<std::function<int()>> {
  static constexpr int value = 0;
};

template <typename Operator, typename Operand>
typename std::enable_if_t<(Precedence<Operand>::value <= Precedence<Operator>::value), std::ostream&>
streamOperand(std::ostream& os, Operand const &operand) {
  Show<Operand>::show(os, operand);
  return os;
}

template <typename Operator, typename Operand>
typename std::enable_if_t<(Precedence<Operand>::value > Precedence<Operator>::value), std::ostream&>
streamOperand(std::ostream& os, Operand const &operand) {
  os << '(';
  Show<Operand>::show(os, operand);
  os << ')';
  return os;
}

} // namespace lazy
