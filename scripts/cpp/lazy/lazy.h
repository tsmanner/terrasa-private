#pragma once

#include <iostream>
#include <functional>
#include <random>

#include "is_callable.h"
#include "operator_functions.h"

inline std::ostream& operator<<(std::ostream& os, std::function<int()> const &) {
  return os << "std::function<int()>";
}

// inline std::ostream& operator<<(std::ostream& os, std::function<int()> const) {
//   return os << "std::function<int()>";
// }

// inline std::ostream& operator<<(std::ostream& os, std::function<int()>) {
//   return os << "std::function<int()>";
// }

// inline std::ostream& operator<<(std::ostream& os, std::function<int()> &&) {
//   return os << "std::function<int()>";
// }

// inline std::ostream& operator<<(std::ostream& os, std::function<int()> &) {
//   return os << "std::function<int()>";
// }

namespace lazy {

template <typename T> struct precedence;

template <>
struct precedence<int> {
  static constexpr int value = 0;
};


template <typename T, T Min, T Max>
struct Random {
  constexpr Random() {}
  T operator()() const {
    static std::random_device rand;
    static std::default_random_engine engine { rand() };
    static std::uniform_int_distribution<T> uniform_distribution { Min, Max };
    return std::forward<T>(uniform_distribution(engine));
  }
};


template <typename T, T Min, T Max>
struct precedence<Random<T, Min, Max>> {
  static constexpr int value = 0;
};


template <typename T, T Min, T Max>
std::ostream& operator<<(std::ostream& os, Random<T, Min, Max> const &) {
  return os << "[" << Min << ":" << Max << "]";
}


//
// Operators
//


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




// // BinaryOperator<std::plus> with a LHS of BinaryOperator<std::plus>
// template <typename LHSLHS, typename LHSRHS, typename RHS>
// struct BinaryOperator<
//   std::plus<>,
//   BinaryOperator<std::plus<>, LHSLHS, LHSRHS>,
//   RHS,
//   typename std::enable_if_t<(LHSLHS)>
// > {
//   constexpr BinaryOperator(BinaryOperator<std::plus<>, LHSLHS, LHSRHS> const &inLHS, RHS const &inRHS): mLHS(inLHS), mRHS(inRHS) {}
//   auto operator()() const { static constexpr auto op = Operator(); return op(mLHS(), mRHS); }
//   BinaryOperator<std::plus<>, LHSLHS, LHSRHS> mLHS;
//   RHS mRHS;
// };





// Operator Precedences
template <typename Operand          > struct precedence<UnaryOperator <lazy::negate,     Operand >> { static constexpr int value =  3; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::plus,       LHS, RHS>> { static constexpr int value =  6; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::minus,      LHS, RHS>> { static constexpr int value =  6; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::multiplies, LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::divides,    LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::modulus,    LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::minimum,    LHS, RHS>> { static constexpr int value = 17; };  // Same as ',' operator
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::maximum,    LHS, RHS>> { static constexpr int value = 17; };  // Same as ',' operator

// Operator Streams
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::negate    >::value), std::ostream&> streamOperator(std::ostream& os) { return os << "-"; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::plus      >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " + "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::minus     >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " - "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::multiplies>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " * "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::divides   >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " / "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::modulus   >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " % "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::minimum   >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " min "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::maximum   >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " max "; }

// Operand Streams
template <typename Operator, typename Operand>
typename std::enable_if_t<(precedence<Operand>::value <= precedence<Operator>::value), std::ostream&>
streamOperand(std::ostream& os, Operand const &operand) {
  return os << operand;
}

template <typename Operator, typename Operand>
typename std::enable_if_t<(precedence<Operand>::value > precedence<Operator>::value), std::ostream&>
streamOperand(std::ostream& os, Operand const &operand) {
  return os << '(' << operand << ')';
}

// Stream Operator
template <typename Operator, typename Operand>
std::ostream& operator<<(std::ostream& os, UnaryOperator<Operator, Operand> const &inOperator) {
  streamOperator<Operator>(os);
  streamOperand<typename std::decay_t<decltype(inOperator)>>(os, inOperator.mOperand);
  return os;
}

template <typename Operator, typename L, typename R>
std::ostream& operator<<(std::ostream& os, BinaryOperator<Operator, L, R> const &inOperator) {
  streamOperand<typename std::decay_t<decltype(inOperator)>>(os, inOperator.mLHS);
  streamOperator<Operator>(os);
  streamOperand<typename std::decay_t<decltype(inOperator)>>(os, inOperator.mRHS);
  return os;
}

// Operator Overloads
template <typename Operand      > constexpr auto operator-(Operand const &operand    ) { return UnaryOperator <lazy::negate,  Operand>(operand ); }
template <typename L, typename R> constexpr auto operator+(L const &lhs, R const &rhs) { return BinaryOperator<lazy::plus,       L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator-(L const &lhs, R const &rhs) { return BinaryOperator<lazy::minus,      L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator*(L const &lhs, R const &rhs) { return BinaryOperator<lazy::multiplies, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator/(L const &lhs, R const &rhs) { return BinaryOperator<lazy::divides,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator%(L const &lhs, R const &rhs) { return BinaryOperator<lazy::modulus,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto min      (L const &lhs, R const &rhs) { return BinaryOperator<lazy::minimum,    L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto max      (L const &lhs, R const &rhs) { return BinaryOperator<lazy::maximum,    L, R>(lhs, rhs); }

}  // namespace lazy
