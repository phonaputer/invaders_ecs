#pragma once

#include <entt.hpp>

namespace invasion {

void add_player_entity(entt::registry &ecs);

entt::entity add_player_projectile_entity(entt::registry &ecs, float x, float y);

void add_player_muzzle_flash_entity(entt::registry &ecs, entt::entity shooter);

void add_player_explosion_entity(entt::registry &ecs, float x, float y, unsigned int lifetime);

} // namespace invasion