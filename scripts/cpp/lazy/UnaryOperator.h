#pragma once

#include <functional>
#include <ostream>

#include "is_callable.h"
#include "operator_functions.h"
#include "Precedence.h"

namespace lazy {

// Default, assumes operand is callable
template <typename Operator, typename Operand, typename=void>
struct UnaryOperator {
  constexpr UnaryOperator(Operand const &inOperand): mOperand(inOperand) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mOperand()); }
  Operand mOperand;
};

// Operand is not callable (e.g. `int`)
template <typename Operator, typename Operand>
struct UnaryOperator<Operator, Operand, typename std::enable_if_t<!is_callable<Operand, int>::value>> {
  constexpr UnaryOperator(Operand const &inOperand): mOperand(inOperand) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mOperand()); }
  Operand mOperand;
};

template <typename Operand          > struct Precedence<UnaryOperator <lazy::negate,     Operand >> { static constexpr int value =  3; };

template <typename Operator, typename Operand>
std::ostream& operator<<(std::ostream& os, UnaryOperator<Operator, Operand> const &inOperator) {
  streamOperator<Operator>(os);
  streamOperand<typename std::decay_t<decltype(inOperator)>>(os, inOperator.mOperand);
  return os;
}

template <typename Operand      > constexpr auto operator-(Operand const &operand    ) { return UnaryOperator <lazy::negate,  Operand>(operand ); }

} // namespace lazy
