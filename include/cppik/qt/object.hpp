#pragma once

namespace cppik { namespace qt {

struct object {

  virtual void update () = 0;

  virtual ~object () noexcept = default;
};

}}
