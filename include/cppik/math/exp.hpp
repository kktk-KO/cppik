#pragma once

#include <cmath>

namespace cppik {

template <class T>
T exp (T const & t) noexcept {
  return std::exp(t);
}

}
