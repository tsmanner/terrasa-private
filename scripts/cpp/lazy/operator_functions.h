#pragma once

#include <functional>
#include <ostream>
#include <fmt/format.h>

#include "Precedence.h"
#include "Show.h"

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

// Operator Precedence
template <> struct Precedence<lazy::negate    > { static constexpr int value =  3; };
template <> struct Precedence<lazy::plus      > { static constexpr int value =  6; };
template <> struct Precedence<lazy::minus     > { static constexpr int value =  6; };
template <> struct Precedence<lazy::multiplies> { static constexpr int value =  5; };
template <> struct Precedence<lazy::divides   > { static constexpr int value =  5; };
template <> struct Precedence<lazy::modulus   > { static constexpr int value =  5; };
template <> struct Precedence<lazy::minimum   > { static constexpr int value = 17; };
template <> struct Precedence<lazy::maximum   > { static constexpr int value = 17; };

// Operator Format
template <> struct Show<lazy::negate    > { static constexpr auto show = "-"; };
template <> struct Show<lazy::plus      > { static constexpr auto show = "+"; };
template <> struct Show<lazy::minus     > { static constexpr auto show = "-"; };
template <> struct Show<lazy::multiplies> { static constexpr auto show = "*"; };
template <> struct Show<lazy::divides   > { static constexpr auto show = "/"; };
template <> struct Show<lazy::modulus   > { static constexpr auto show = "%"; };
template <> struct Show<lazy::minimum   > { static constexpr auto show = "min"; };
template <> struct Show<lazy::maximum   > { static constexpr auto show = "max"; };

} // namespace lazy

