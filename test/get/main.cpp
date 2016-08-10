#include <cppik/utility/get.hpp>

#include <array>
#include <cassert>
#include <vector>

int main () {
  assert(cppik::get(1, 0) == 1);
  assert(cppik::get(std::array<int, 2>{0, 1}, 1) == 1);
  assert(cppik::get(std::string("abc"), 1) == 'b');
}
