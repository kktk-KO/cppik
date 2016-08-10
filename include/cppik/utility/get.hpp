#pragma once

#include <cppik/utility/value_type.hpp>
#include <cppik/utility/size.hpp>
#include <cppik/utility/size_type.hpp>
#include <cppik/utility/enable_if.hpp>

#include <cassert>
#include <utility>

namespace cppik {

namespace impl {

template <class T>
struct has_operator_bracket {
private:
  using U = typename std::remove_cv<typename std::remove_reference<T>::type>::type;

  template <class S>
  static auto check (S && s, cppik::size_type<U> i, int) -> decltype(std::forward<S>(s)[i], std::true_type());

  template <class S>
  static auto check (S && s, ...) -> std::false_type;
public:
  using type = bool;
  static constexpr bool value = decltype(check(std::declval<T>(), std::declval<cppik::size_type<U> const &>(), 42))::value; 
};

template <class T>
using return_type_operator_bracket = decltype(
  std::declval<T>()[
    std::declval<cppik::size_type<T> const &>()
  ]
);

}

template <class T, class SizeType, CPPIK_ENABLE_IF(!impl::has_operator_bracket<T &&>::value)>
T && get (T && t, SizeType i) noexcept {
  assert(i == 0);
  return std::forward<T>(t);
}

template <class T, class SizeType, CPPIK_ENABLE_IF(impl::has_operator_bracket<T &>::value)>
impl::return_type_operator_bracket<T> get (T && t, SizeType i) noexcept {
  assert(0 <= i && i < size(t));
  return t[i];
}

}
