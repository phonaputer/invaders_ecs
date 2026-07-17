#pragma once

#include "framework/scene.hpp"
#include "framework/scene_initialization_context.hpp"

namespace invasion {

class Scene : public framework::Scene {
    void initialize(framework::SceneInitializationContext ctx) override;
};

} // namespace invasion