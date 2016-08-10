#pragma once

#include <type_traits>

namespace cppik {

template <class T>
struct always_true : std::true_type {};

}
