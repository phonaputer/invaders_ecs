#pragma once

#include <flecs.h>

namespace framework {

class SystemAdder {
  public:
    virtual ~SystemAdder() = default;
    virtual void add_draw_system(flecs::system system) = 0;
};

} // namespace framework
