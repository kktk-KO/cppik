#pragma once

#include <type_traits>

namespace cppik {

extern void * enabler ;

}

#define CPPIK_ENABLE_IF(expr) \
  typename std::enable_if<expr>::type *& = cppik::enabler
