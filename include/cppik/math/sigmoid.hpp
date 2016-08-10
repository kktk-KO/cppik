#pragma once

#include <cppik/math/exp.hpp>

namespace cppik {

template <class T>
T sigmoid (T const & t) noexcept {
  return 1 / (1 + exp(-t));
}

template <class T>
T sigmoid (T const & a, T const & t) noexcept {
  return 1 / (1 + exp(-a * t));
}

}
