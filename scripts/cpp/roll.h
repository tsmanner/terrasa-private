#pragma once

#include <iostream>
#include <functional>
#include <random>


template <typename T>
struct is_callable {
private:
  template <typename _T, int (_T::*)() const = &_T::operator()>
  static constexpr bool check(_T *) {
    return true;
  }
  template <typename... Ts> static constexpr bool check(Ts&&...) {
    return false;
  }

public:
  static constexpr bool value = check(static_cast<T *>(nullptr));

};


namespace lazy {

  template <typename=void>
  struct minimum {
    constexpr minimum() {}
    template <typename L, typename R>
    constexpr auto operator()(L const &lhs, R const &rhs) const {
      return (lhs < rhs) ? lhs : rhs;
    }
  };

  template <typename=void>
  struct maximum {
    constexpr maximum() {}
    template <typename L, typename R>
    constexpr auto operator()(L const &lhs, R const &rhs) const {
      return (lhs > rhs) ? lhs : rhs;
    }
  };

}



template <typename T> struct precedence;

template <>
struct precedence<int> {
  static constexpr int value = 0;
};


template <int Sides>
struct Die {
  constexpr Die() {}
  int operator()() const {
    static std::random_device rand;
    static std::default_random_engine engine { rand() };
    static std::uniform_int_distribution<int> uniform_distribution { 1, Sides };
    return std::forward<int>(uniform_distribution(engine));
  }
};


template <int Sides>
struct precedence<Die<Sides>> {
  static constexpr int value = 0;
};


template <int Sides>
std::ostream& operator<<(std::ostream& os, Die<Sides> const &) {
  return os << "d" << Sides;
}


static constinit auto d20 = Die<20>();
static constinit auto d12 = Die<12>();
static constinit auto d10 = Die<10>();
static constinit auto  d8 = Die< 8>();
static constinit auto  d6 = Die< 6>();
static constinit auto  d4 = Die< 4>();


//
// Operations
// - Add
// - Subtract
// - Max (advantage)
// - Min (disadvantage)
// - Halve (floor(N / 2))
// - Double (N * 2)
// - Greater Than or Equal To
// - Less Than
//


// Default, assumes operand is callable
template <typename Operator, typename Operand, typename=void>
struct UnaryOperator {
  constexpr UnaryOperator(Operand const &inOperand): mOperand(inOperand) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mOperand()); }
  Operand mOperand;
};

// Operand is not callable (e.e. `int`)
template <typename Operator, typename Operand>
struct UnaryOperator<Operator, Operand, typename std::enable_if_t<!is_callable<Operand>::value>> {
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

// LHS is not callable (i.e. `int`)
template <typename Operator, typename LHS, typename RHS>
struct BinaryOperator<Operator, LHS, RHS, typename std::enable_if_t<(!is_callable<LHS>::value)>> {
  constexpr BinaryOperator(LHS const &inLHS, RHS const &inRHS): mLHS(inLHS), mRHS(inRHS) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mLHS, mRHS()); }
  LHS mLHS;
  RHS mRHS;
};

// RHS is not callable (i.e. `int`)
template <typename Operator, typename LHS, typename RHS>
struct BinaryOperator<Operator, LHS, RHS, typename std::enable_if_t<(!is_callable<RHS>::value)>> {
  constexpr BinaryOperator(LHS const &inLHS, RHS const &inRHS): mLHS(inLHS), mRHS(inRHS) {}
  auto operator()() const { static constexpr auto op = Operator(); return op(mLHS(), mRHS); }
  LHS mLHS;
  RHS mRHS;
};


// Operator Precedences
template <typename Operand          > struct precedence<UnaryOperator <std::negate    <>, Operand >> { static constexpr int value =  3; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<std::plus      <>, LHS, RHS>> { static constexpr int value =  6; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<std::minus     <>, LHS, RHS>> { static constexpr int value =  6; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<std::multiplies<>, LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<std::divides   <>, LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<std::modulus   <>, LHS, RHS>> { static constexpr int value =  5; };
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::minimum  <>, LHS, RHS>> { static constexpr int value = 17; };  // Same as ',' operator
template <typename LHS, typename RHS> struct precedence<BinaryOperator<lazy::maximum  <>, LHS, RHS>> { static constexpr int value = 17; };  // Same as ',' operator

// Operator Streams
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, std::negate    <>>::value), std::ostream&> streamOperator(std::ostream& os) { return os << "-"; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, std::plus      <>>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " + "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, std::minus     <>>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " - "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, std::multiplies<>>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " * "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, std::divides   <>>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " / "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, std::modulus   <>>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " % "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::minimum  <>>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " min "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::maximum  <>>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " max "; }

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
template <typename Operand      > constexpr auto operator-(Operand const &operand    ) { return UnaryOperator <std::negate    <>, Operand>(operand); }
template <typename L, typename R> constexpr auto operator+(L const &lhs, R const &rhs) { return BinaryOperator<std::plus      <>, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator-(L const &lhs, R const &rhs) { return BinaryOperator<std::minus     <>, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator*(L const &lhs, R const &rhs) { return BinaryOperator<std::multiplies<>, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator/(L const &lhs, R const &rhs) { return BinaryOperator<std::divides   <>, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto operator%(L const &lhs, R const &rhs) { return BinaryOperator<std::modulus   <>, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto min      (L const &lhs, R const &rhs) { return BinaryOperator<lazy::minimum  <>, L, R>(lhs, rhs); }
template <typename L, typename R> constexpr auto max      (L const &lhs, R const &rhs) { return BinaryOperator<lazy::maximum  <>, L, R>(lhs, rhs); }
