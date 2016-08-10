#pragma once

#include <cppik/utility/enable_if.hpp>
#include <cppik/utility/size_type.hpp>

#include <type_traits>

namespace cppik {

namespace impl {

template <class T>
struct has_size_memfn {
private:
  template <class S>
  static auto check (S const & s, int) -> decltype(s.size(), std::true_type());

  template <class S>
  static auto check (S const & s, ...) -> std::false_type;
public:
  using type = bool;
  static constexpr bool value = decltype(check(std::declval<T const &>(), 42))::value; 
};

}

template <class T, CPPIK_ENABLE_IF(!impl::has_size_memfn<T>::value)>
size_type<T> size (T const & t) noexcept {
  return 1;
}

template <class T, CPPIK_ENABLE_IF(impl::has_size_memfn<T>::value)>
size_type<T> size (T const & t) noexcept {
  return t.size();
}

}
