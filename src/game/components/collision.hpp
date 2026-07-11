#pragma once

#include "framework/ecs/entity.hpp"

namespace components {

struct Collision {
    float hitbox_offset_x;
    float hitbox_offset_y;
    float hitbox_w;
    float hitbox_h;
};

} // namespace components