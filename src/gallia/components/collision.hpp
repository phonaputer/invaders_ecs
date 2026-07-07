#pragma once

#include "framework/ecs/entity.hpp"
#include <string>

namespace components {

namespace collision {
enum class Type { Unspecified, Player, Alien, PlayerProjectile, AlienProjectile, Fortress };
};

struct Collision {
    bool active; // TODO is this needed?

    collision::Type type;
    float hitbox_offset_x;
    float hitbox_offset_y;
    float hitbox_w;
    float hitbox_h;

    // allowing only one hit per entity per frame is pretty low-tech but
    // should work fine for this use case - and it means hits can be
    // registered here in the ECS
    bool hit_something_this_tick = false;
    ecs::Entity who_i_hit = 0;
    collision::Type type_of_who_i_hit = collision::Type::Unspecified;
};

} // namespace components