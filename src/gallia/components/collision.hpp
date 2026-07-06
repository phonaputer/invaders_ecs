#pragma once

#include <vector>

namespace components {

enum class CollisionType { Unspecified, Player, Alien, PlayerProjectile, AlienProjectile };

struct Collision {
    bool active;

    CollisionType type;
    float hitbox_offset_x;
    float hitbox_offset_y;
    float hitbox_w;
    float hitbox_h;

    bool hit_something_this_tick;
    CollisionType type_of_what_i_hit;
};

} // namespace components