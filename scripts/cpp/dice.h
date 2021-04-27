#pragma once

#include "lazy/lazy.h"

template <int Sides>
struct Die : public lazy::Random<int, 1, Sides> {
  using lazy::Random<int, 1, Sides>::Random;
  auto operator()() const { return lazy::Random<int, 1, Sides>::operator()(); }
};

namespace lazy {
  template <int Sides>
  struct precedence<Die<Sides>> {
    static constexpr int value = 0;
  };
}  // namespace lazy

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
