#pragma once

#include <functional>

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

} // namespace lazy

