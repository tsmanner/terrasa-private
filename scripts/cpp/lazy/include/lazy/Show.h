#pragma once

#include <functional>
#include <ostream>

namespace lazy {

template <typename T> struct Show {
  static void show(std::ostream& os, T const &t) { os << t; }
};

template <> struct Show<std::function<int()>> {
  static void show(std::ostream& os, std::function<int()> const &) { os << "f->int"; }
};

} // namespace lazy
