#pragma once

#include "framework/ecs/ecs.hpp"
#include "framework/game/asset_loader.hpp"

namespace game {

struct SceneInitializationContext {
    AssetLoader &assets;
    ecs::ECS &ecs;
};

} // namespace game