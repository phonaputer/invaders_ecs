#pragma once

#include "core/point.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"

namespace game {

void add_player_entity(ecs::ECS &ecs);

ecs::Entity add_player_projectile_entity(ecs::ECS &ecs, core::Point starting_point);

void add_player_muzzle_flash_entity(ecs::ECS &ecs, ecs::Entity shooter);

void add_player_explosion_entity(ecs::ECS &ecs, core::Point position, unsigned int lifetime);

} // namespace game