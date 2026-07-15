#pragma once

#include "framework/system.hpp"

namespace systems {

class SpriteOffsetOnDamage : public framework::System {
  public:
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems