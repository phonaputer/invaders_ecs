#pragma once

#include "core/point.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"

namespace gallia {

ecs::Entity add_player_projectile_entity(ecs::ECS &ecs, core::Point starting_point);

} // namespace gallia