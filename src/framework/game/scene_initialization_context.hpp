#pragma once

#include "framework/ecs/ecs.hpp"
#include "framework/game/asset_loader.hpp"
#include "framework/game/player_input_manager.hpp"
#include "framework/game/renderer.hpp"

namespace game {

struct SceneInitializationContext {
    AssetLoader &assets;
    ecs::ECS &ecs;
    Renderer &renderer;
    PlayerInputManager &player_input_manager;
};

} // namespace game