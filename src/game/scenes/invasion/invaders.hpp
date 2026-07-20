#pragma once

#include <entt.hpp>
#include <random>

namespace invasion {

void add_eel_entity(entt::registry &ecs, std::mt19937 &rand_gen);

void add_invader_entities(entt::registry &ecs);

void add_invader_projectile(entt::registry &ecs, float x, float y);

void add_explosion(entt::registry &ecs, float x, float y);

} // namespace invasion