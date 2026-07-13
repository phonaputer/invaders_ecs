#pragma once

#include "framework/scene_initialization_context.hpp"

namespace framework {

class Scene {
  public:
    virtual ~Scene() = default;
    virtual void initialize(SceneInitializationContext ctx) = 0;
};

} // namespace framework