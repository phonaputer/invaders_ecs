#pragma once

#include "framework/ecs/ecs.hpp"
#include "framework/game/asset_manager.hpp"

namespace game {

struct SceneInitializationContext {
    AssetManager &assets;
    ecs::ECS &ecs;
};

} // namespace game