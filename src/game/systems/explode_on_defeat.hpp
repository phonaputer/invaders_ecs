#pragma once

#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include <flecs.h>
#include <functional>

namespace systems {

void explode_on_defeat(flecs::world world, std::function<void(flecs::world, core::Point)> add_explosion);

} // namespace systems