#pragma once

#include "framework/asset_loader.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/renderer.hpp"

namespace game {

struct SceneInitializationContext {
    AssetLoader &assets;
    ecs::ECS &ecs;
    Renderer &renderer;
    PlayerInputManager &player_input_manager;
};

} // namespace game