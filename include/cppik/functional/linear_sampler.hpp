#pragma once

#include <cppik/core.hpp>
#include <cppik/math.hpp>

#include <algorithm>
#include <vector>

namespace cppik {

template <class ValueType>
struct linear_sampler {

  using size_type = std::size_t;
  using value_type = ValueType;

  template <class Vec1, class Vec2>
  void operator() (Vec1 const & vec1, Vec2 & vec2) {
    static_assert(std::is_same<cppik::value_type<Vec1>, cppik::value_type<Vec2>>::value, "");
    static_assert(std::is_same<cppik::value_type<cppik::value_type<Vec1>>, value_type>::value, "");
    static_assert(std::is_same<cppik::value_type<cppik::value_type<Vec2>>, value_type>::value, "");

    assert(size(vec1) > 0);

    if (size(vec2) == 0) { return; }

    get(vec2, 0) = get(vec1, 0);

    if (size(vec2) == 1) { return; }

    get(vec2, vec2.size() - 1) = get(vec1, vec1.size() - 1);

    len_.resize(size(vec1));
    len_[0] = 0;

    for (cppik::size_type<Vec1> i = 1; i < size(vec1); ++i) {
      len_[i] = sqrt(norm(vec1[i], vec1[i - 1])) + len_[i - 1];
    }

    auto first = len_.begin();
    auto last = len_.end();
    auto s = size(vec2);
    value_type L = len_.back();
    value_type dL = L / (vec2.size() - 1);
    for (cppik::size_type<Vec2> i = 1; i < s - 1; ++i) {
      value_type v = i * dL;

      first = std::upper_bound(first, last, v);
      value_type L1 = first[-1];
      value_type L2 = first[0];
      value_type P = (v - L1) / (L2 - L1);
      cppik::size_type<Vec1> j = std::distance(len_.begin(), first);

      // vec2[i] = vec1[j - 1] * P + vec1[j] * (1 - P)
      map(get(vec2, i), vec1[j - 1], vec1[j], [&] (value_type & a, value_type b, value_type c) {
        a = b * (1 - P) + c * P;
      });
    }
  }

private:
  std::vector<value_type> len_;
};

}
