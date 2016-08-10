#include <cppik/utility/size_type.hpp>

#include <array>
#include <cassert>
#include <type_traits>
#include <vector>

struct foo {
};

struct bar {
  using size_type = int;
};

namespace cppik { namespace impl {

// customization point
template <>
struct size_type<bar, true> {
  using type = std::size_t;
};

}}

int main () {
  // size_type<int> == std::size_t
  assert((std::is_same<cppik::size_type<int>, std::size_t>::value));
  // size_type<std::vector<int>> == std::size_t
  assert((std::is_same<cppik::size_type<std::vector<int>>, std::size_t>::value));
  // size_type<foo> == std::size_t
  assert((std::is_same<cppik::size_type<foo>, std::size_t>::value));
  // size_type<bar> == std::size_t
  assert((std::is_same<cppik::size_type<foo>, std::size_t>::value));

}
