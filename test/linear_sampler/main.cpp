#include <cppik/functional/linear_sampler.hpp>

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

int main () {
  using Vec2f = std::array<float, 2>;
  std::array<Vec2f, 4> a{
    Vec2f{0, 0},
    Vec2f{1, 1},
    Vec2f{2, 2},
    Vec2f{4, 4}
  };

  std::array<Vec2f, 4> b;
  cppik::linear_sampler<float> sampler;
  sampler(a, b);

  for (Vec2f const & v : b) { std::cout << v[0] << ", " << v[1] << std::endl; }
}
