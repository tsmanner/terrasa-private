#pragma once

#include <ostream>
#include <random>

#include "Precedence.h"

namespace lazy {


namespace random {
  // Source of randomness
  static std::random_device rand;
  // Random engine
  static std::default_random_engine engine { rand() };
  // Uniform distribution
  template <int Min, int Max, template <typename> typename DistributionType>
  static DistributionType<int>
    distribution { Min, Max };
}  // namespace random


template <int Min, int Max,
  template <typename> typename DistributionType = std::uniform_int_distribution>
struct Random {
  constexpr Random() {}

  int operator()() const {
    return std::forward<int>(random::distribution<Min, Max, DistributionType>(random::engine));
  }
};


template <int Value, template <typename> typename DistributionType>
struct Random<Value, Value, DistributionType> {
  constexpr Random() {}

  int operator()() const {
    return Value;
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
