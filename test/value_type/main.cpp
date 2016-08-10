#include <cppik/utility/value_type.hpp>

#include <array>
#include <cassert>
#include <type_traits>
#include <vector>

struct foo {
  using value_type = double;
};

namespace cppik { namespace impl {

// customization point
template <>
struct value_type<foo, true> {
  using type = int;
};

}}

int main () {
  // value_type<int> == int
  assert((std::is_same<cppik::value_type<int>, int>::value));
  // value_type<std::vector<int>> == int
  assert((std::is_same<cppik::value_type<std::vector<int>>, int>::value));
  // value_type<foo> == int
  assert((std::is_same<cppik::value_type<foo>, int>::value));
}
