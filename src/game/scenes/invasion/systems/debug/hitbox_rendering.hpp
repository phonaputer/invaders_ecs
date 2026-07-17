#pragma once

#include "framework/renderer.hpp"
#include "framework/system.hpp"

namespace systems::debug {

class HitboxRendering : public framework::System {
  private:
    framework::Renderer &renderer;

  public:
    HitboxRendering(framework::Renderer &renderer);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems::debug