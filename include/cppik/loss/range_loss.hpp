#pragma once

#include <cppik/math/abs.hpp>
#include <cppik/math/sigmoid.hpp>

namespace cppik {

template <class ValueType>
struct range_loss {

  using value_type = ValueType;

  range_loss (value_type min, value_type max, value_type beta = 1000, value_type gamma = 1000) noexcept
  : min_(min),
    max_(max),
    center_((min + max) / 2),
    range_(max - center_),
    beta_(beta),
    gamma_(gamma) {
  }

  value_type operator() (value_type v) const noexcept {
    return gamma_ * sigmoid(beta_, abs(v - center_) - range_);
  } 

private:
  ValueType min_;
  ValueType max_;
  value_type center_;
  value_type range_;
  value_type beta_;
  value_type gamma_;
};

}
