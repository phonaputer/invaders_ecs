#pragma once

#include "framework/system.hpp"

namespace systems::player {

class Movement : public framework::System {
  public:
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems