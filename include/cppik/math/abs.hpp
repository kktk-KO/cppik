#pragma once

#include <cmath>

namespace cppik {

template <class T>
T abs (T const & t) noexcept {
  return std::abs(t);
}

}
