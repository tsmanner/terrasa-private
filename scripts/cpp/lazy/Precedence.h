#pragma once

#include <functional>

namespace lazy {

template <typename T>
struct Precedence;

template <>
struct Precedence<int> {
  static constexpr int value = 0;
};

template <>
struct Precedence<std::function<int()>> {
  static constexpr int value = 0;
};

} // namespace lazy
