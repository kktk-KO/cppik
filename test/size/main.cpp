#include <cppik/utility/size.hpp>

#include <array>
#include <cassert>
#include <vector>

int main () {
  assert(cppik::size(1) == 1);
  assert(cppik::size(1.0) == 1);
  assert(cppik::size("a") == 1);
  assert(cppik::size(std::vector<int>(10)) == 10);
  assert(cppik::size(std::array<int, 10>()) == 10);
}
