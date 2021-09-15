#pragma once

template <typename T, typename RT>
struct is_callable {
private:
  template <typename _T, RT (_T::*)() const = &_T::operator()>
  static constexpr bool check(_T *) {
    return true;
  }
  template <typename... Ts> static constexpr bool check(Ts&&...) {
    return false;
  }

public:
  static constexpr bool value = check(static_cast<T *>(nullptr));

};

