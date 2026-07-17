#pragma once

#include "framework/system.hpp"

namespace systems::invaders {

class Landed : public framework::System {
  public:
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems::invaders