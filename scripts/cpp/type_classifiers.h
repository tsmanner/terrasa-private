#pragma once

#include <type_traits>


namespace typing {

//
// Type
//

template <typename T>
struct Type {
  using type = T;

  constexpr Type<T>() {}

};


namespace detail {


//
// Combinators
//

// EQUAL
template <typename...> struct TypeEQUAL;

// NOT
template <typename T>
struct TypeNOT {
  using type = Type<T>;

  constexpr TypeNOT<T>() {}
  // constexpr operator!
};

// NOT Equal
template <typename TL, typename TR>
struct TypeNOT<TypeEQUAL<TL, TR>> {
  using type = TypeEQUAL<TL, TR>;

  constexpr TypeNOT() {}

  constexpr operator bool() const {
    return !static_cast<bool>(type());
  }
};

// OR
template <typename TL, typename TR>
struct TypeOR {
  using type_left = TL;
  using type_right = TR;

  constexpr TypeOR<TL, TR>() {}
};

// AND
template <typename TL, typename TR>
struct TypeAND {
  using type_left = TL;
  using type_right = TR;

  constexpr TypeAND<TL, TR>() {}
};


//
// Equality
//

// Type Type
template <typename TL, typename TR>
struct TypeEQUAL<Type<TL>, Type<TR>> {
  using type_left = Type<TL>;
  using type_right = Type<TR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return std::is_same<typename type_left::type, typename type_right::type>::value;
  }
};


// Type TypeNOT
template <typename TL, typename TR>
struct TypeEQUAL<Type<TL>, TypeNOT<TR>> {
  using type_left = Type<TL>;
  using type_right = TypeNOT<TR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return !static_cast<bool>(type_left() == typename type_right::type());
  }
};


// Type TypeOR
template <typename TL, typename TRL, typename TRR>
struct TypeEQUAL<Type<TL>, TypeOR<TRL, TRR>> {
  using type_left = Type<TL>;
  using type_right = TypeOR<TRL, TRR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(type_left() == typename type_right::type_left())
        || static_cast<bool>(type_left() == typename type_right::type_right());
  }
};

// Type TypeAND
template <typename TL, typename TRL, typename TRR>
struct TypeEQUAL<Type<TL>, TypeAND<TRL, TRR>> {
  using type_left = Type<TL>;
  using type_right = TypeAND<TRL, TRR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(type_left() == typename type_right::type_left())
        && static_cast<bool>(type_left() == typename type_right::type_right());
  }
};

// TypeOR Type
template <typename TLL, typename TLR, typename TR>
struct TypeEQUAL<TypeOR<TLL, TLR>, Type<TR>> {
  using type_left = TypeOR<TLL, TLR>;
  using type_right = Type<TR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(typename type_left::type_left() == type_right())
        || static_cast<bool>(typename type_left::type_right() == type_right());
  }
};

// TypeOR TypeNOT
template <typename TLL, typename TLR, typename TR>
struct TypeEQUAL<TypeOR<TLL, TLR>, TypeNOT<TR>> {
  using type_left = TypeOR<TLL, TLR>;
  using type_right = TypeNOT<TR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(typename type_left::type_left() == type_right())
        || static_cast<bool>(typename type_left::type_right() == type_right());
  }
};


// TypeOR TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
struct TypeEQUAL<TypeOR<TLL, TLR>, TypeOR<TRL, TRR>> {
  using type_left = TypeOR<TLL, TLR>;
  using type_right = TypeOR<TRL, TRR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(typename type_left::type_left() == type_right())
        || static_cast<bool>(typename type_left::type_right() == type_right());
  }
};

// TypeOR TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
struct TypeEQUAL<TypeOR<TLL, TLR>, TypeAND<TRL, TRR>> {
  using type_left = TypeOR<TLL, TLR>;
  using type_right = TypeAND<TRL, TRR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(typename type_left::type_left() == type_right())
        || static_cast<bool>(typename type_left::type_right() == type_right());
  }
};

// TypeAND Type
template <typename TLL, typename TLR, typename TR>
struct TypeEQUAL<TypeAND<TLL, TLR>, Type<TR>> {
  using type_left = TypeAND<TLL, TLR>;
  using type_right = Type<TR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(typename type_left::type_left() == type_right())
        && static_cast<bool>(typename type_left::type_right() == type_right());
  }
};

// TypeAND TypeNOT
template <typename TLL, typename TLR, typename TR>
struct TypeEQUAL<TypeAND<TLL, TLR>, TypeNOT<TR>> {
  using type_left = TypeAND<TLL, TLR>;
  using type_right = TypeNOT<TR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(typename type_left::type_left() == type_right())
        && static_cast<bool>(typename type_left::type_right() == type_right());
  }
};

// TypeAND TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
struct TypeEQUAL<TypeAND<TLL, TLR>, TypeOR<TRL, TRR>> {
  using type_left = TypeAND<TLL, TLR>;
  using type_right = TypeOR<TRL, TRR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(typename type_left::type_left() == type_right())
        && static_cast<bool>(typename type_left::type_right() == type_right());
  }
};

// TypeAND TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
struct TypeEQUAL<TypeAND<TLL, TLR>, TypeAND<TRL, TRR>> {
  using type_left = TypeAND<TLL, TLR>;
  using type_right = TypeAND<TRL, TRR>;

  constexpr TypeEQUAL() {}

  constexpr operator bool() const {
    return static_cast<bool>(typename type_left::type_left() == type_right())
        && static_cast<bool>(typename type_left::type_right() == type_right());
  }
};


}  // namespace typing::detail


//
// EQUAL
//

// Type Type
template <typename TL, typename TR>
constexpr auto operator==(Type<TL>, Type<TR>) -> decltype(detail::TypeEQUAL<Type<TL>, Type<TR>>()) {
  return detail::TypeEQUAL<Type<TL>, Type<TR>>();
}
// Type TypeNOT
template <typename TL, typename TR>
constexpr auto operator==(Type<TL>, detail::TypeNOT<TR>) -> decltype(detail::TypeEQUAL<Type<TL>, detail::TypeNOT<TR>>()) {
  return detail::TypeEQUAL<Type<TL>, detail::TypeNOT<TR>>();
}
// Type TypeOR
template <typename TL, typename TRL, typename TRR>
constexpr auto operator==(Type<TL>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeEQUAL<Type<TL>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeEQUAL<Type<TL>, detail::TypeOR<TRL, TRR>>();
}
// Type TypeAND
template <typename TL, typename TRL, typename TRR>
constexpr auto operator==(Type<TL>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeEQUAL<Type<TL>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeEQUAL<Type<TL>, detail::TypeAND<TRL, TRR>>();
}
// TypeNOT Type
template <typename TL, typename TR>
constexpr auto operator==(detail::TypeNOT<TL>, Type<TR>) -> decltype(detail::TypeEQUAL<detail::TypeNOT<TL>, Type<TR>>()) {
  return detail::TypeEQUAL<detail::TypeNOT<TL>, Type<TR>>();
}
// TypeNOT TypeNOT
template <typename TL, typename TR>
constexpr auto operator==(detail::TypeNOT<TL>, detail::TypeNOT<TR>) -> decltype(detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeNOT<TR>>()) {
  return detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeNOT<TR>>();
}
// TypeNOT TypeOR
template <typename TL, typename TRL, typename TRR>
constexpr auto operator==(detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>>();
}
// TypeNOT TypeAND
template <typename TL, typename TRL, typename TRR>
constexpr auto operator==(detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>>();
}
// TypeOR Type
template <typename TLL, typename TLR, typename TR>
constexpr auto operator==(detail::TypeOR<TLL, TLR>, Type<TR>) -> decltype(detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, Type<TR>>()) {
  return detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, Type<TR>>();
}
// TypeOR TypeNOT
template <typename TLL, typename TLR, typename TR>
constexpr auto operator==(detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>) -> decltype(detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>>()) {
  return detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>>();
}
// TypeOR TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator==(detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>>();
}
// TypeOR TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator==(detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>>();
}
// TypeAND Type
template <typename TLL, typename TLR, typename TR>
constexpr auto operator==(detail::TypeAND<TLL, TLR>, Type<TR>) -> decltype(detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, Type<TR>>()) {
  return detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, Type<TR>>();
}
// TypeAND TypeNOT
template <typename TLL, typename TLR, typename TR>
constexpr auto operator==(detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>) -> decltype(detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>>()) {
  return detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>>();
}
// TypeAND TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator==(detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>>();
}
// TypeAND TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator==(detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>>();
}


//
// NOT EQUAL
//

// Type Type
template <typename TL, typename TR>
constexpr auto operator!=(Type<TL>, Type<TR>) -> decltype(!detail::TypeEQUAL<Type<TL>, Type<TR>>()) {
  return !detail::TypeEQUAL<Type<TL>, Type<TR>>();
}
// Type TypeNOT
template <typename TL, typename TR>
constexpr auto operator!=(Type<TL>, detail::TypeNOT<TR>) -> decltype(!detail::TypeEQUAL<Type<TL>, detail::TypeNOT<TR>>()) {
  return !detail::TypeEQUAL<Type<TL>, detail::TypeNOT<TR>>();
}
// Type TypeOR
template <typename TL, typename TRL, typename TRR>
constexpr auto operator!=(Type<TL>, detail::TypeOR<TRL, TRR>) -> decltype(!detail::TypeEQUAL<Type<TL>, detail::TypeOR<TRL, TRR>>()) {
  return !detail::TypeEQUAL<Type<TL>, detail::TypeOR<TRL, TRR>>();
}
// Type TypeAND
template <typename TL, typename TRL, typename TRR>
constexpr auto operator!=(Type<TL>, detail::TypeAND<TRL, TRR>) -> decltype(!detail::TypeEQUAL<Type<TL>, detail::TypeAND<TRL, TRR>>()) {
  return !detail::TypeEQUAL<Type<TL>, detail::TypeAND<TRL, TRR>>();
}
// TypeNOT Type
template <typename TL, typename TR>
constexpr auto operator!=(detail::TypeNOT<TL>, Type<TR>) -> decltype(!detail::TypeEQUAL<detail::TypeNOT<TL>, Type<TR>>()) {
  return !detail::TypeEQUAL<detail::TypeNOT<TL>, Type<TR>>();
}
// TypeNOT TypeNOT
template <typename TL, typename TR>
constexpr auto operator!=(detail::TypeNOT<TL>, detail::TypeNOT<TR>) -> decltype(!detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeNOT<TR>>()) {
  return !detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeNOT<TR>>();
}
// TypeNOT TypeOR
template <typename TL, typename TRL, typename TRR>
constexpr auto operator!=(detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>) -> decltype(!detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>>()) {
  return !detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>>();
}
// TypeNOT TypeAND
template <typename TL, typename TRL, typename TRR>
constexpr auto operator!=(detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>) -> decltype(!detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>>()) {
  return !detail::TypeEQUAL<detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>>();
}
// TypeOR Type
template <typename TLL, typename TLR, typename TR>
constexpr auto operator!=(detail::TypeOR<TLL, TLR>, Type<TR>) -> decltype(!detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, Type<TR>>()) {
  return !detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, Type<TR>>();
}
// TypeOR TypeNOT
template <typename TLL, typename TLR, typename TR>
constexpr auto operator!=(detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>) -> decltype(!detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>>()) {
  return !detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>>();
}
// TypeOR TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator!=(detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>) -> decltype(!detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>>()) {
  return !detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>>();
}
// TypeOR TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator!=(detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>) -> decltype(!detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>>()) {
  return !detail::TypeEQUAL<detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>>();
}
// TypeAND Type
template <typename TLL, typename TLR, typename TR>
constexpr auto operator!=(detail::TypeAND<TLL, TLR>, Type<TR>) -> decltype(!detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, Type<TR>>()) {
  return !detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, Type<TR>>();
}
// TypeAND TypeNOT
template <typename TLL, typename TLR, typename TR>
constexpr auto operator!=(detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>) -> decltype(!detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>>()) {
  return !detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>>();
}
// TypeAND TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator!=(detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>) -> decltype(!detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>>()) {
  return !detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>>();
}
// TypeAND TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator!=(detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>) -> decltype(!detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>>()) {
  return !detail::TypeEQUAL<detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>>();
}


//
// NOT
//

// Type
template <typename T>
constexpr auto operator!(Type<T>) -> decltype(detail::TypeNOT<Type<T>>()) {
  return detail::TypeNOT<Type<T>>();
}
// TypeNOT
template <typename T>
constexpr auto operator!(detail::TypeNOT<T>) -> decltype(T()) {
  return T();
}
// TypeEQUAL
template <typename TL, typename TR>
constexpr auto operator!(detail::TypeEQUAL<TL, TR>) -> decltype(detail::TypeNOT<detail::TypeEQUAL<TL, TR>>()) {
  return detail::TypeNOT<detail::TypeEQUAL<TL, TR>>();
}
// TypeOR
template <typename TL, typename TR>
constexpr auto operator!(detail::TypeOR<TL, TR>) -> decltype(detail::TypeNOT<detail::TypeOR<TL, TR>>()) {
  return detail::TypeNOT<detail::TypeOR<TL, TR>>();
}
// TypeAND
template <typename TL, typename TR>
constexpr auto operator!(detail::TypeAND<TL, TR>) -> decltype(detail::TypeNOT<detail::TypeAND<TL, TR>>()) {
  return detail::TypeNOT<detail::TypeAND<TL, TR>>();
}


//
// OR
//

// Type Type
template <typename TL, typename TR>
constexpr auto operator||(Type<TL>, Type<TR>) -> decltype(detail::TypeOR<Type<TL>, Type<TR>>()) {
  return detail::TypeOR<Type<TL>, Type<TR>>();
}
// Type TypeNOT
template <typename TL, typename TR>
constexpr auto operator||(Type<TL>, detail::TypeNOT<TR>) -> decltype(detail::TypeOR<Type<TL>, detail::TypeNOT<TR>>()) {
  return detail::TypeOR<Type<TL>, detail::TypeNOT<TR>>();
}
// Type TypeOR
template <typename TL, typename TRL, typename TRR>
constexpr auto operator||(Type<TL>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeOR<Type<TL>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeOR<Type<TL>, detail::TypeOR<TRL, TRR>>();
}
// Type TypeAND
template <typename TL, typename TRL, typename TRR>
constexpr auto operator||(Type<TL>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeOR<Type<TL>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeOR<Type<TL>, detail::TypeAND<TRL, TRR>>();
}
// TypeNOT Type
template <typename TL, typename TR>
constexpr auto operator||(detail::TypeNOT<TL>, Type<TR>) -> decltype(detail::TypeOR<detail::TypeNOT<TL>, Type<TR>>()) {
  return detail::TypeOR<detail::TypeNOT<TL>, Type<TR>>();
}
// TypeNOT TypeNOT
template <typename TL, typename TR>
constexpr auto operator||(detail::TypeNOT<TL>, detail::TypeNOT<TR>) -> decltype(detail::TypeOR<detail::TypeNOT<TL>, detail::TypeNOT<TR>>()) {
  return detail::TypeOR<detail::TypeNOT<TL>, detail::TypeNOT<TR>>();
}
// TypeNOT TypeOR
template <typename TL, typename TRL, typename TRR>
constexpr auto operator||(detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeOR<detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeOR<detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>>();
}
// TypeNOT TypeAND
template <typename TL, typename TRL, typename TRR>
constexpr auto operator||(detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeOR<detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeOR<detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>>();
}
// TypeOR Type
template <typename TLL, typename TLR, typename TR>
constexpr auto operator||(detail::TypeOR<TLL, TLR>, Type<TR>) -> decltype(detail::TypeOR<detail::TypeOR<TLL, TLR>, Type<TR>>()) {
  return detail::TypeOR<detail::TypeOR<TLL, TLR>, Type<TR>>();
}
// TypeOR TypeNOT
template <typename TLL, typename TLR, typename TR>
constexpr auto operator||(detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>) -> decltype(detail::TypeOR<detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>>()) {
  return detail::TypeOR<detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>>();
}
// TypeOR TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator||(detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeOR<detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeOR<detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>>();
}
// TypeOR TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator||(detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeOR<detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeOR<detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>>();
}
// TypeAND Type
template <typename TLL, typename TLR, typename TR>
constexpr auto operator||(detail::TypeAND<TLL, TLR>, Type<TR>) -> decltype(detail::TypeOR<detail::TypeAND<TLL, TLR>, Type<TR>>()) {
  return detail::TypeOR<detail::TypeAND<TLL, TLR>, Type<TR>>();
}
// TypeAND TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator||(detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeOR<detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeOR<detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>>();
}
// TypeAND TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator||(detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeOR<detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeOR<detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>>();
}


//
// AND
//

// Type Type
template <typename TL, typename TR>
constexpr auto operator&&(Type<TL>, Type<TR>) -> decltype(detail::TypeAND<Type<TL>, Type<TR>>()) {
  return detail::TypeAND<Type<TL>, Type<TR>>();
}
// Type TypeNOT
template <typename TL, typename TR>
constexpr auto operator&&(Type<TL>, detail::TypeNOT<TR>) -> decltype(detail::TypeAND<Type<TL>, detail::TypeNOT<TR>>()) {
  return detail::TypeAND<Type<TL>, detail::TypeNOT<TR>>();
}
// Type TypeOR
template <typename TL, typename TRL, typename TRR>
constexpr auto operator&&(Type<TL>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeAND<Type<TL>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeAND<Type<TL>, detail::TypeOR<TRL, TRR>>();
}
// Type TypeAND
template <typename TL, typename TRL, typename TRR>
constexpr auto operator&&(Type<TL>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeAND<Type<TL>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeAND<Type<TL>, detail::TypeAND<TRL, TRR>>();
}
// TypeNOT Type
template <typename TL, typename TR>
constexpr auto operator&&(detail::TypeNOT<TL>, Type<TR>) -> decltype(detail::TypeAND<detail::TypeNOT<TL>, Type<TR>>()) {
  return detail::TypeAND<detail::TypeNOT<TL>, Type<TR>>();
}
// TypeNOT TypeNOT
template <typename TL, typename TR>
constexpr auto operator&&(detail::TypeNOT<TL>, detail::TypeNOT<TR>) -> decltype(detail::TypeAND<detail::TypeNOT<TL>, detail::TypeNOT<TR>>()) {
  return detail::TypeAND<detail::TypeNOT<TL>, detail::TypeNOT<TR>>();
}
// TypeNOT TypeOR
template <typename TL, typename TRL, typename TRR>
constexpr auto operator&&(detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeAND<detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeAND<detail::TypeNOT<TL>, detail::TypeOR<TRL, TRR>>();
}
// TypeNOT TypeAND
template <typename TL, typename TRL, typename TRR>
constexpr auto operator&&(detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeAND<detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeAND<detail::TypeNOT<TL>, detail::TypeAND<TRL, TRR>>();
}
// TypeOR Type
template <typename TLL, typename TLR, typename TR>
constexpr auto operator&&(detail::TypeOR<TLL, TLR>, Type<TR>) -> decltype(detail::TypeAND<detail::TypeOR<TLL, TLR>, Type<TR>>()) {
  return detail::TypeAND<detail::TypeOR<TLL, TLR>, Type<TR>>();
}
// TypeOR TypeNOT
template <typename TLL, typename TLR, typename TR>
constexpr auto operator&&(detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>) -> decltype(detail::TypeAND<detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>>()) {
  return detail::TypeAND<detail::TypeOR<TLL, TLR>, detail::TypeNOT<TR>>();
}
// TypeOR TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator&&(detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeAND<detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeAND<detail::TypeOR<TLL, TLR>, detail::TypeOR<TRL, TRR>>();
}
// TypeOR TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator&&(detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeAND<detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeAND<detail::TypeOR<TLL, TLR>, detail::TypeAND<TRL, TRR>>();
}
// TypeAND Type
template <typename TLL, typename TLR, typename TR>
constexpr auto operator&&(detail::TypeAND<TLL, TLR>, Type<TR>) -> decltype(detail::TypeAND<detail::TypeAND<TLL, TLR>, Type<TR>>()) {
  return detail::TypeAND<detail::TypeAND<TLL, TLR>, Type<TR>>();
}
// TypeAND TypeNOT
template <typename TLL, typename TLR, typename TR>
constexpr auto operator&&(detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>) -> decltype(detail::TypeAND<detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>>()) {
  return detail::TypeAND<detail::TypeAND<TLL, TLR>, detail::TypeNOT<TR>>();
}
// TypeAND TypeOR
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator&&(detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>) -> decltype(detail::TypeAND<detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>>()) {
  return detail::TypeAND<detail::TypeAND<TLL, TLR>, detail::TypeOR<TRL, TRR>>();
}
// TypeAND TypeAND
template <typename TLL, typename TLR, typename TRL, typename TRR>
constexpr auto operator&&(detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>) -> decltype(detail::TypeAND<detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>>()) {
  return detail::TypeAND<detail::TypeAND<TLL, TLR>, detail::TypeAND<TRL, TRR>>();
}

}  // namespace typing
