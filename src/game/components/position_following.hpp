#pragma once

#include "framework/ecs/entity.hpp"

namespace components {

struct PositionFollowing {
    ecs::Entity leader;
    float x_offset;
    float y_offset;
};

} // namespace components