#pragma once

#include "core/point.hpp"
#include <entt.hpp>

namespace invasion {

void add_invader_entities(entt::registry &ecs);

void add_invader_projectile(entt::registry &ecs, core::Point starting_point);

void add_explosion(entt::registry &ecs, core::Point position);

} // namespace invasion