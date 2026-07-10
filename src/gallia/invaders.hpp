#pragma once

#include "core/point.hpp"
#include "framework/ecs/ecs.hpp"

namespace gallia {

void add_invader_entities(ecs::ECS &ecs);

void add_invader_projectile(ecs::ECS &ecs, core::Point starting_point);

} // namespace gallia