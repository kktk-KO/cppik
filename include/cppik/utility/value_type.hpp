#pragma once

#include <type_traits>

namespace cppik {

namespace impl {

template <class T>
struct has_value_type_mem {
private:
  template <class S>
  static auto check (S & s, int) -> decltype(std::declval<typename S::value_type>(), std::true_type());

  template <class S>
  static auto check (S & s, ...) -> std::false_type;
public:
  using type = bool;
  static constexpr bool value = decltype(check(std::declval<T &>(), 42))::value; 
};

template <class T, bool HasValueType>
struct value_type;

template <class T>
struct value_type<T, true> {
  using type = typename T::value_type;
};

template <class T>
struct value_type<T, false> {
  using type = T;
};

}

template <class T>
using value_type = typename impl::value_type<
  typename std::remove_reference<
    typename std::remove_cv<T>::type
  >::type,
  impl::has_value_type_mem<T>::value
>::type;

}
