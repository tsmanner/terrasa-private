#pragma once

#include <ostream>
#include <random>
#include <fmt/format.h>

#include "Precedence.h"

namespace lazy {

template <int Min, int Max>
struct Random {
  constexpr Random() {}
  int operator()() const {
    static std::random_device rand;
    static std::default_random_engine engine { rand() };
    static std::uniform_int_distribution<int> uniform_distribution { Min, Max };
    return std::forward<int>(uniform_distribution(engine));
  }
};


template <int Min, int Max>
struct Precedence<Random<Min, Max>> {
  static constexpr int value = 0;
};


template <int Min, int Max>
std::ostream& operator<<(std::ostream& os, Random<Min, Max> const &) {
  return os << "[" << Min << ":" << Max << "]";
}

} // namespace lazy
