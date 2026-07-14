#pragma once

#include "framework/asset_loader.hpp"
#include "framework/renderer.hpp"
#include "framework/system_registry.hpp"

#include <entt.hpp>

namespace framework {

struct SceneInitializationContext {
    AssetLoader &assets;
    SystemRegistry &systems;
    entt::registry &ecs;
    Renderer &renderer;
};

} // namespace framework