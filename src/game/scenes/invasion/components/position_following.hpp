#pragma once

#include <entt.hpp>

namespace components {

struct PositionFollowing {
    entt::entity leader;
    float x_offset;
    float y_offset;
};

} // namespace components