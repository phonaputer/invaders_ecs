#pragma once

#include "framework/system.hpp"
#include <set>

namespace systems {

class Velocity : public framework::System {
  private:
    bool paused = false;

  public:
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems