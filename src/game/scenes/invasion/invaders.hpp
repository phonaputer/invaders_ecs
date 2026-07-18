#pragma once

#include <entt.hpp>

namespace invasion {

void add_invader_entities(entt::registry &ecs);

void add_invader_projectile(entt::registry &ecs, float x, float y);

void add_explosion(entt::registry &ecs, float x, float y);

} // namespace invasion