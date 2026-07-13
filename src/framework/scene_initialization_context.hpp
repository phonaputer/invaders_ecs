#pragma once

#include "framework/asset_loader.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/player_input_manager.hpp"
#include "framework/renderer.hpp"
#include <flecs.h>

namespace framework {

struct SceneInitializationContext {
    AssetLoader &assets;
    flecs::world &world;
    Renderer &renderer;
    PlayerInputManager &player_input_manager;
};

} // namespace framework