#pragma once

#include <string>

namespace components {

struct Collision {
    bool active;

    std::string type;
    float hitbox_offset_x;
    float hitbox_offset_y;
    float hitbox_w;
    float hitbox_h;

    bool hit_something_this_tick;
    std::string type_of_what_i_hit;
};

} // namespace components