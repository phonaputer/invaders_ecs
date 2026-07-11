#pragma once

#include "framework/scene_initialization_context.hpp"

namespace game {

class Scene {
  public:
    virtual ~Scene() = default;
    virtual void initialize(SceneInitializationContext ctx) = 0;
};

} // namespace game