#pragma once

#include <cppik/core.hpp>

namespace cppik {

template <class T>
value_type<T> norm (T const & t) noexcept {
  value_type<T> sum = 0;
  for (size_type<T> i = 0; i < size(t); ++i) {
    sum += get(t, i) * get(t, i);
  }
  return sum;
}

template <class T1, class T2>
value_type<T1> norm (T1 const & t1, T2 const & t2) noexcept {
  static_assert(std::is_same<value_type<T1>, value_type<T2>>::value, "");
  assert(size(t1) == size(t2));

  value_type<T1> sum = 0;
  for (size_type<T1> i = 0; i < size(t1); ++i) {
    value_type<T1> v = get(t1, i) - get(t2, i);
    sum += v * v;
  }
  return sum;
}

}
