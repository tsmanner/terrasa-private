#pragma once

#include <functional>
#include <ostream>

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

template <typename LHS, typename RHS> struct Precedence<BinaryOperator<lazy::plus,       LHS, RHS>> { static constexpr int value =  6; };
template <typename LHS, typename RHS> struct Precedence<BinaryOperator<lazy::minus,      LHS, RHS>> { static constexpr int value =  6; };
template <typename LHS, typename RHS> struct Precedence<BinaryOperator<lazy::multiplies, LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct Precedence<BinaryOperator<lazy::divides,    LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct Precedence<BinaryOperator<lazy::modulus,    LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct Precedence<BinaryOperator<lazy::minimum,    LHS, RHS>> { static constexpr int value = 17; };  // Same as ',' operator
template <typename LHS, typename RHS> struct Precedence<BinaryOperator<lazy::maximum,    LHS, RHS>> { static constexpr int value = 17; };  // Same as ',' operator

template <typename Operator, typename L, typename R>
std::ostream& operator<<(std::ostream& os, BinaryOperator<Operator, L, R> const &inOperator) {
  streamOperand<typename std::decay_t<decltype(inOperator)>>(os, inOperator.mLHS);
  streamOperator<Operator>(os);
  streamOperand<typename std::decay_t<decltype(inOperator)>>(os, inOperator.mRHS);
  return os;
}

template <typename L, typename R> constexpr auto operator+(L const &lhs, R const &rhs) { return BinaryOperator<lazy::plus,       L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator-(L const &lhs, R const &rhs) { return BinaryOperator<lazy::minus,      L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator*(L const &lhs, R const &rhs) { return BinaryOperator<lazy::multiplies, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator/(L const &lhs, R const &rhs) { return BinaryOperator<lazy::divides,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator%(L const &lhs, R const &rhs) { return BinaryOperator<lazy::modulus,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto min      (L const &lhs, R const &rhs) { return BinaryOperator<lazy::minimum,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto max      (L const &lhs, R const &rhs) { return BinaryOperator<lazy::maximum,    L, R>(lhs, rhs); }

} // namespace lazy
