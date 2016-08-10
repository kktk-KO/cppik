#pragma once

#include <cppik/utility/get.hpp>
#include <cppik/utility/size.hpp>
#include <cppik/utility/size_type.hpp>
#include <cppik/utility/value_type.hpp>

namespace cppik {

template <class T, class F>
void map (T && t, F && f) {
  size_type<T> e = size(t);
  for (size_type<T> i = 0; i < e; ++i) {
    f(get(t, i));
  }
}

template <class T1, class T2, class F>
void map (T1 && t1, T2 && t2, F && f) {
  assert(size(t1) == size(t2));
  size_type<T1> e = size(t1);
  for (size_type<T1> i = 0; i < e; ++i) {
    f(get(t1, i), get(t2, i));
  }
}

template <class T1, class T2, class T3, class F>
void map (T1 && t1, T2 && t2, T3 && t3, F && f) {
  assert(size(t1) == size(t2));
  assert(size(t2) == size(t3));
  size_type<T1> e = size(t1);
  for (size_type<T1> i = 0; i < e; ++i) {
    f(get(t1, i), get(t2, i), get(t3, i));
  }
}

}
