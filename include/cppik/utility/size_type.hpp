#pragma once

#include <type_traits>

namespace cppik {

namespace impl {

template <class T>
struct has_size_type_mem {
private:
  template <class S>
  static auto check (S & s, int) -> decltype(std::declval<typename S::size_type>(), std::true_type());

  template <class S>
  static auto check (S & s, ...) -> std::false_type;
public:
  using type = bool;
  static constexpr bool value = decltype(check(std::declval<T &>(), 42))::value; 
};

template <class T, bool HasSizeType>
struct size_type;

template <class T>
struct size_type<T, true> {
  using type = typename T::size_type;
};

template <class T>
struct size_type<T, false> {
  using type = std::size_t;
};

}

/**
 *  @details
 *  If default_size_type is modified, modify impl::size_type<T, false> too.
 */
using default_size_type = std::size_t;

template <class T>
using size_type = typename impl::size_type<
  typename std::remove_reference<
    typename std::remove_cv<T>::type
  >::type,
  impl::has_size_type_mem<T>::value
>::type;

}
