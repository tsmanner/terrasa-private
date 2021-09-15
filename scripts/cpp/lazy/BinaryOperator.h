#pragma once

#include <functional>
#include <ostream>
#include <string_view>
#include <fmt/format.h>


#include "is_callable.h"
#include "operator_functions.h"
#include "Precedence.h"

namespace lazy {

// Default, assumes both sides are callable
template <typename Operator, typename LHS, typename RHS, typename=void>
struct BinaryOperator {
  constexpr BinaryOperator(LHS const &inLHS, RHS const &inRHS): mLHS(inLHS), mRHS(inRHS) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mLHS(), mRHS()); }
  LHS mLHS;
  RHS mRHS;
};

// LHS is not callable (e.g. `int`)
template <typename Operator, typename LHS, typename RHS>
struct BinaryOperator<Operator, LHS, RHS, typename std::enable_if_t<(!is_callable<LHS, int>::value)>> {
  constexpr BinaryOperator(LHS const &inLHS, RHS const &inRHS): mLHS(inLHS), mRHS(inRHS) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mLHS, mRHS()); }
  LHS mLHS;
  RHS mRHS;
};

// RHS is not callable (e.g. `int`)
template <typename Operator, typename LHS, typename RHS>
struct BinaryOperator<Operator, LHS, RHS, typename std::enable_if_t<(!is_callable<RHS, int>::value)>> {
  constexpr BinaryOperator(LHS const &inLHS, RHS const &inRHS): mLHS(inLHS), mRHS(inRHS) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mLHS(), mRHS); }
  LHS mLHS;
  RHS mRHS;
};

// Precedence
template <typename Operator, typename LHS, typename RHS> struct Precedence<BinaryOperator<Operator, LHS, RHS>> : public Precedence<Operator> {};

// Operator Overloads
template <typename L, typename R> constexpr auto operator+(L const &lhs, R const &rhs) { return BinaryOperator<lazy::plus,       L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator-(L const &lhs, R const &rhs) { return BinaryOperator<lazy::minus,      L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator*(L const &lhs, R const &rhs) { return BinaryOperator<lazy::multiplies, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator/(L const &lhs, R const &rhs) { return BinaryOperator<lazy::divides,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator%(L const &lhs, R const &rhs) { return BinaryOperator<lazy::modulus,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto min      (L const &lhs, R const &rhs) { return BinaryOperator<lazy::minimum,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto max      (L const &lhs, R const &rhs) { return BinaryOperator<lazy::maximum,    L, R>(lhs, rhs); }

} // namespace lazy


template <typename Operator, typename L, typename R>
struct fmt::formatter<lazy::BinaryOperator<Operator, L, R>> : formatter<std::string_view> {
  template <typename Operand>
  inline typename std::enable_if_t<(lazy::Precedence<Operand>::value <= lazy::Precedence<Operator>::value), std::string>
  formatOperand(Operand const &inOperand) {
    return fmt::format("{}", inOperand);
  }

  template <typename Operand>
  inline typename std::enable_if_t<(lazy::Precedence<Operand>::value > lazy::Precedence<Operator>::value), std::string>
  formatOperand(Operand const &inOperand) {
    return fmt::format("({})", inOperand);
  }

  template <typename FormatContext>
  auto format(lazy::BinaryOperator<Operator, L, R> const &inOperator, FormatContext &ctx) {
    return formatter<std::string_view>::format(fmt::format("{} {} {}",
      formatOperand(inOperator.mLHS),
      lazy::Show<Operator>::show,
      formatOperand(inOperator.mRHS)
    ), ctx);
  }
};

