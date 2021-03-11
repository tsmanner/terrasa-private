#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <type_traits>

static const std::string nl = "\n";


struct Atom {};


template <typename T> struct NoOp {
  // Const Reference NoOp
  constexpr const T& operator()(const T& inT) const { return inT; }
  // Forwarded rvalue NoOp
  constexpr auto operator()(T&& inT) const noexcept(noexcept(std::forward<T>(inT))) -> decltype(std::forward<T>(inT)) { return std::forward<T>(inT); }
};


template <int MIN, int MAX> struct Roll {
  int operator()() const {
    static std::random_device rand;
    static std::default_random_engine engine { rand() };
    static std::uniform_int_distribution<int> uniform_distribution { MIN, MAX };
    return std::forward<int>(uniform_distribution(engine));
  }
  int max() const { return MAX; }
};

template <typename T> struct Literal {
  const T mT;
  Literal(const T& inT): mT(inT) {}

  int operator()() const { return mT; }
  int max() const { return mT; }
};


template <typename Operation, typename Source> struct UnaryAtom {
  const Source mSource {};
  const Operation mOperation {};
  UnaryAtom() {}
  UnaryAtom(const Source& inSource): mSource(inSource) {}
  UnaryAtom(Source&& inSource): mSource(inSource) {}

  int operator()() const { return mOperation(mSource()); }
  int max() const { return mOperation(mSource.max()); }
};


template <typename Operation, typename SourceL, typename SourceR> struct BinaryAtom {
  const SourceL mSourceL {};
  const SourceR mSourceR {};
  const Operation mOperation {};
  BinaryAtom() {}
  BinaryAtom(const SourceL& inSourceL, const SourceR& inSourceR): mSourceL(inSourceL), mSourceR(inSourceR) {}
  BinaryAtom(SourceL&& inSourceL, SourceR&& inSourceR): mSourceL(inSourceL), mSourceR(inSourceR) {}

  int operator()() const { return mOperation(mSourceL(), mSourceR()); }
  int max() const { return mOperation(mSourceL.max(), mSourceR.max()); }
};


template <typename T> auto atom(const T& inT) { return UnaryAtom<NoOp<int>, T>(inT); }
auto atom(const int& i) { return atom(Literal<int>(i)); }



//
// Negate
//

template <typename Operation, typename Source>
UnaryAtom<std::negate<int>, UnaryAtom<Operation, Source>>
operator-(const UnaryAtom<Operation, Source>& rhs) {
  return UnaryAtom<std::negate<int>, UnaryAtom<Operation, Source>>(rhs);
}

template <typename Operation, typename SourceL, typename SourceR>
UnaryAtom<std::negate<int>, BinaryAtom<Operation, SourceL, SourceR>>
operator-(const BinaryAtom<Operation, SourceL, SourceR>& rhs) {
  return UnaryAtom<std::negate<int>, BinaryAtom<Operation, SourceL, SourceR>>(rhs);
}


//
// UnaryAtom UnaryAtom
//

template <typename lOperation, typename lSource, typename rOperation, typename rSource>
auto operator+(const UnaryAtom<lOperation, lSource>& lhs, const UnaryAtom<rOperation, rSource>& rhs) {
  return BinaryAtom<std::plus<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSource, typename rOperation, typename rSource>
auto operator-(const UnaryAtom<lOperation, lSource>& lhs, const UnaryAtom<rOperation, rSource>& rhs) {
  return BinaryAtom<std::minus<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSource, typename rOperation, typename rSource>
auto operator*(const UnaryAtom<lOperation, lSource>& lhs, const UnaryAtom<rOperation, rSource>& rhs) {
  return BinaryAtom<std::multiplies<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSource, typename rOperation, typename rSource>
auto operator/(const UnaryAtom<lOperation, lSource>& lhs, const UnaryAtom<rOperation, rSource>& rhs) {
  return BinaryAtom<std::divides<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}


//
// UnaryAtom BinaryAtom
//

template <typename lOperation, typename lSource, typename rOperation, typename rSourceL, typename rSourceR>
auto operator+(const UnaryAtom<lOperation, lSource>& lhs, const BinaryAtom<rOperation, rSourceL, rSourceR>& rhs) {
  return BinaryAtom<std::plus<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSource, typename rOperation, typename rSourceL, typename rSourceR>
auto operator-(const UnaryAtom<lOperation, lSource>& lhs, const BinaryAtom<rOperation, rSourceL, rSourceR>& rhs) {
  return BinaryAtom<std::minus<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSource, typename rOperation, typename rSourceL, typename rSourceR>
auto operator*(const UnaryAtom<lOperation, lSource>& lhs, const BinaryAtom<rOperation, rSourceL, rSourceR>& rhs) {
  return BinaryAtom<std::multiplies<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSource, typename rOperation, typename rSourceL, typename rSourceR>
auto operator/(const UnaryAtom<lOperation, lSource>& lhs, const BinaryAtom<rOperation, rSourceL, rSourceR>& rhs) {
  return BinaryAtom<std::divides<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}


//
// BinaryAtom UnaryAtom
//

template <typename lOperation, typename lSourceL, typename lSourceR, typename rOperation, typename rSource>
auto operator+(const BinaryAtom<lOperation, lSourceL, lSourceR>& lhs, const UnaryAtom<rOperation, rSource>& rhs) {
  return BinaryAtom<std::plus<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSourceL, typename lSourceR, typename rOperation, typename rSource>
auto operator-(const BinaryAtom<lOperation, lSourceL, lSourceR>& lhs, const UnaryAtom<rOperation, rSource>& rhs) {
  return BinaryAtom<std::minus<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSourceL, typename lSourceR, typename rOperation, typename rSource>
auto operator*(const BinaryAtom<lOperation, lSourceL, lSourceR>& lhs, const UnaryAtom<rOperation, rSource>& rhs) {
  return BinaryAtom<std::multiplies<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSourceL, typename lSourceR, typename rOperation, typename rSource>
auto operator/(const BinaryAtom<lOperation, lSourceL, lSourceR>& lhs, const UnaryAtom<rOperation, rSource>& rhs) {
  return BinaryAtom<std::divides<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}


//
// BinaryAtom BinaryAtom
//

template <typename lOperation, typename lSourceL, typename lSourceR, typename rOperation, typename rSourceL, typename rSourceR>
auto operator+(const BinaryAtom<lOperation, lSourceL, lSourceR>& lhs, const BinaryAtom<rOperation, rSourceL, rSourceR>& rhs) {
  return BinaryAtom<std::plus<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSourceL, typename lSourceR, typename rOperation, typename rSourceL, typename rSourceR>
auto operator-(const BinaryAtom<lOperation, lSourceL, lSourceR>& lhs, const BinaryAtom<rOperation, rSourceL, rSourceR>& rhs) {
  return BinaryAtom<std::minus<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSourceL, typename lSourceR, typename rOperation, typename rSourceL, typename rSourceR>
auto operator*(const BinaryAtom<lOperation, lSourceL, lSourceR>& lhs, const BinaryAtom<rOperation, rSourceL, rSourceR>& rhs) {
  return BinaryAtom<std::multiplies<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}

template <typename lOperation, typename lSourceL, typename lSourceR, typename rOperation, typename rSourceL, typename rSourceR>
auto operator/(const BinaryAtom<lOperation, lSourceL, lSourceR>& lhs, const BinaryAtom<rOperation, rSourceL, rSourceR>& rhs) {
  return BinaryAtom<std::divides<int>, std::decay_t<decltype(lhs)>, std::decay_t<decltype(rhs)>>(lhs, rhs);
}


//
// Non-Atom Operators
//

template <typename T>
auto operator-(const T& rhs) {
  return -atom(rhs);
}

template <typename T1, typename T2>
auto operator+(const T1& lhs, const T2& rhs) {
  return atom(lhs) + atom(rhs);
}

template <typename T1, typename T2>
auto operator-(const T1& lhs, const T2& rhs) {
  return atom(lhs) - atom(rhs);
}

template <typename T1, typename T2>
auto operator*(const T1& lhs, const T2& rhs) {
  return atom(lhs) * atom(rhs);
}

template <typename T1, typename T2>
auto operator/(const T1& lhs, const T2& rhs) {
  return atom(lhs) / atom(rhs);
}


template <int Count, typename T>
constexpr
typename std::enable_if<(Count > 1), auto>::type
N(const T& inT)
-> decltype(inT + N<Count-1>(inT))
{

}


using D4  = Roll<1,  4>; static auto  d4 =  D4();
using D6  = Roll<1,  6>; static auto  d6 =  D6();
using D8  = Roll<1,  8>; static auto  d8 =  D8();
using D10 = Roll<1, 10>; static auto d10 = D10();
using D12 = Roll<1, 12>; static auto d12 = D12();
using D20 = Roll<1, 20>; static auto d20 = D20();


struct AttackRoll {
  const int mBonus;
  AttackRoll(decltype(mBonus)& inBonus): mBonus(inBonus) {}

  int operator()() const {
    auto value = d20();
    switch (value) {
      case  1: return std::numeric_limits<int>::min();
      case 20: return std::numeric_limits<int>::max();
      default: return value + mBonus;
    }
  }
};


struct Entity {
  const int mAttackBonus;
  const std::pair<int, int> mPosition;
  const uint8_t mAC;
  Entity(
    decltype(mAttackBonus)& inAttackBonus,
    decltype(mPosition)& inPosition,
    decltype(mAC)& inAC
  ):
    mAttackBonus(inAttackBonus),
    mPosition(inPosition),
    mAC(inAC)
  {}

  int attack(const Entity& target) const {
    auto dr = d4 + d6;
    auto distance = std::hypot(target.mPosition.first - mPosition.first, target.mPosition.second - mPosition.second);
    auto attack_roll = AttackRoll(mAttackBonus)();
    if (attack_roll == std::numeric_limits<int>::min()) { return 0; }
    else if (attack_roll == std::numeric_limits<int>::max()) { return dr.max() + dr(); }
    else if (attack_roll >= target.mAC) { return dr(); }
    return 0;
  }

};


struct Attack {
  const AttackRoll mAttackRoll;
  const uint8_t mTargetAC;
  Attack(
    decltype(mAttackRoll)& inAttackRoll,
    decltype(mTargetAC)& inTargetAC
  ):
    mAttackRoll(inAttackRoll),
    mTargetAC(inTargetAC)
  {}


  template <typename DamageRoll>
  int operator()(const DamageRoll& dr) const {
    auto attack_roll = mAttackRoll();
    if (attack_roll == std::numeric_limits<int>::min()) { return 0; }
    else if (attack_roll == std::numeric_limits<int>::max()) { return dr.max() + dr(); }
    else if (attack_roll >= mTargetAC) { return dr(); }
    return 0;
  }

};


int main() {
  using std::cout;
  auto attack = Attack(AttackRoll(5), 15);
  for (int i = 0; i < 20; ++i) {
    cout << attack(d4 + d6 + d6 + 4) << nl;
  }
  return 0;
}
