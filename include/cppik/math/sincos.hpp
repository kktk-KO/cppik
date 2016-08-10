#pragma once

#include <cmath>
#include <utility>
#include <math.h>

namespace cppik {

template <class T>
std::pair<T, T> sincos (T const & t) noexcept { 
  return std::pair<T, T>(std::sin(t), std::cos(t));
}

std::pair<double, double> sincos (double const & t) noexcept {
  double s;
  double c;
  ::sincos(t, &s, &c);
  return std::pair<double, double>(s, c);
}


std::pair<float, float> sincos (float const & t) noexcept {
  float s;
  float c;
  ::sincosf(t, &s, &c);
  return std::pair<float, float>(s, c);
}

}
