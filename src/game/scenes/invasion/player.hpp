#pragma once

#include "core/point.hpp"
#include <entt.hpp>

namespace invasion {

void add_player_entity(entt::registry &ecs);

entt::entity add_player_projectile_entity(entt::registry &ecs, core::Point starting_point);

void add_player_muzzle_flash_entity(entt::registry &ecs, entt::entity shooter);

void add_player_explosion_entity(entt::registry &ecs, core::Point position, unsigned int lifetime);

} // namespace invasion