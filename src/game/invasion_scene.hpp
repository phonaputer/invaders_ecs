#pragma once

#include "framework/scene.hpp"
#include "framework/scene_initialization_context.hpp"

namespace gallia {

class InvasionScene : public framework::Scene {
    void initialize(framework::SceneInitializationContext ctx) override;
};

} // namespace gallia