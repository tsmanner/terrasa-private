#pragma once

#include <functional>
#include <ostream>

namespace lazy {

template <typename T> struct Show;

template <> struct Show<std::function<int()>> {
  static constexpr auto show = "f->int";
};

} // namespace lazy
