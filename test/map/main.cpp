#include <cppik/utility/map.hpp>

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

int main () {
  std::vector<int> a;
  // a = {0}
  cppik::map(0, [&] (int i) { a.push_back(i); });
  // a = {0, 1, 2}
  cppik::map(std::array<int, 2>{1, 2}, [&] (int const & i) { a.push_back(i); });
  // a = {0, 1, 2}, b = {1, 2}
  std::vector<int> b{1, 2};
  // a = {0, 1, 2}, b = {3, 4}
  cppik::map(b, [] (int & i) { i += 2; });
  // a = {0, 1, 2, 3, 4}
  cppik::map(b, [&] (int const & i) { a.push_back(i); });

  for (std::size_t i = 0; i < a.size(); ++i) {
    assert(a[i] == i);
  }
}
