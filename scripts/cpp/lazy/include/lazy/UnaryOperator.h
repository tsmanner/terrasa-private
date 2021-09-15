#pragma once

#include <concepts>
#include <functional>
#include <ostream>
#include <fmt/format.h>

#include "operator_functions.h"
#include "Precedence.h"

namespace lazy {

// Default, assumes operand is callable
template <typename Operator, typename Operand, typename=void>
struct UnaryOperator;


template <typename Operator, std::invocable Operand>
struct UnaryOperator<Operator, Operand> {
  constexpr UnaryOperator(Operand const &inOperand): mOperand(inOperand) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mOperand()); }
  Operand mOperand;
};

// Operand is not callable (e.g. `int`)
template <typename Operator, typename Operand>
struct UnaryOperator<Operator, Operand> {
  constexpr UnaryOperator(Operand const &inOperand): mOperand(inOperand) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mOperand); }
  Operand mOperand;
};

template <typename Operand>
struct Precedence<UnaryOperator<lazy::negate, Operand>> {
  static constexpr int value = 3;
};

template <typename Operator, typename Operand>
std::ostream& operator<<(std::ostream& os, UnaryOperator<Operator, Operand> const &inOperator) {
  streamOperator<Operator>(os);
  streamOperand<typename std::decay_t<decltype(inOperator)>>(os, inOperator.mOperand);
  return os;
}

template <typename Operand> constexpr auto operator-(Operand const &operand) { return UnaryOperator<lazy::negate, Operand>(operand); }

} // namespace lazy

template <typename Operator, typename Operand>
struct fmt::formatter<lazy::UnaryOperator<Operator, Operand>> : formatter<std::string_view> {
  template <typename FormatContext>
  auto format(lazy::UnaryOperator<Operator, Operand> const &inOperator, FormatContext &ctx) {
    return formatter<std::string_view>::format(fmt::format("{}{}",
      lazy::Show<Operator>::show,
      inOperator.mOperand
    ), ctx);
  }
};
