#pragma once

#include "framework/scene.hpp"
#include "framework/scene_initialization_context.hpp"

namespace gallia {

class InvasionScene : public game::Scene {
    void initialize(game::SceneInitializationContext ctx) override;
};

} // namespace gallia