#pragma once

#include "framework/game/scene.hpp"
#include "framework/game/scene_initialization_context.hpp"

namespace gallia {

class InvasionScene : public game::Scene {
    void initialize(game::SceneInitializationContext ctx) override;
};

} // namespace gallia