#pragma once

#include "framework/renderer.hpp"
#include "framework/system.hpp"

namespace systems {

class Rendering : public framework::System {
  private:
    framework::Renderer &renderer;

  public:
    Rendering(framework::Renderer &renderer);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems