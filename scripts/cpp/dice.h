#pragma once

#include <sstream>
#include <string_view>
#include <fmt/color.h>
#include <fmt/format.h>

#include "lazy/lazy.h"

template <int Sides>
struct Die : public lazy::Random<1, Sides> {
  using lazy::Random<1, Sides>::Random;
  auto operator()() const { return lazy::Random<1, Sides>::operator()(); }
};

namespace lazy {
  template <int Sides>
  struct Precedence<Die<Sides>> {
    static constexpr int value = 0;
  };
}  // namespace lazy

template <int Sides>
struct fmt::formatter<Die<Sides>> {
  constexpr format_parse_context::iterator parse(
      format_parse_context& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(Die<Sides> const &, FormatContext &ctx) {
    auto out = ctx.out();
    out = fmt::format_to(out, fmt::emphasis::bold, "d{}", Sides);
    return out;
  }
};


static constinit auto d20 = Die<20>();
static constinit auto d12 = Die<12>();
static constinit auto d10 = Die<10>();
static constinit auto  d8 = Die< 8>();
static constinit auto  d6 = Die< 6>();
static constinit auto  d4 = Die< 4>();
