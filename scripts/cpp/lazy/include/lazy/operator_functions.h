#pragma once

#include <functional>
#include <ostream>

namespace lazy {

using negate     = std::negate    <>;
using plus       = std::plus      <>;
using minus      = std::minus     <>;
using multiplies = std::multiplies<>;
using divides    = std::divides   <>;
using modulus    = std::modulus   <>;

struct minimum {
  constexpr minimum() {}
  template <typename L, typename R>
  constexpr auto operator()(L const &lhs, R const &rhs) const {
    return (lhs < rhs) ? lhs : rhs;
  }
};

struct maximum {
  constexpr maximum() {}
  template <typename L, typename R>
  constexpr auto operator()(L const &lhs, R const &rhs) const {
    return (lhs > rhs) ? lhs : rhs;
  }
};

// Operator Streams
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::negate    >::value), std::ostream&> streamOperator(std::ostream& os) { return os << "-"; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::plus      >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " + "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::minus     >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " - "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::multiplies>::value), std::ostream&> streamOperator(std::ostream& os) { return os << " * "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::divides   >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " / "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::modulus   >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " % "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::minimum   >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " min "; }
template <typename Operator> typename std::enable_if_t<(std::is_same<Operator, lazy::maximum   >::value), std::ostream&> streamOperator(std::ostream& os) { return os << " max "; }

} // namespace lazy

