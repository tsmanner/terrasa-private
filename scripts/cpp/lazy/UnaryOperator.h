#pragma once

#include <functional>
#include <ostream>
#include <fmt/format.h>

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

// Precedence
template <typename Operator, typename Operand> struct Precedence<UnaryOperator<Operator, Operand>> : public Precedence<Operator> {};

// Operator Overloads
template <typename Operand> constexpr auto operator-(Operand const &operand) { return UnaryOperator <lazy::negate, Operand>(operand ); }

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
