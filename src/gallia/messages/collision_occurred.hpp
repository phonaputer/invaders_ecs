#pragma once

#include "framework/ecs/entity.hpp"

namespace messages {

struct CollisionOccurred {
    ecs::Entity who_am_i;
    ecs::Entity who_i_hit;

    bool operator==(const CollisionOccurred &) const = default;
};

} // namespace messages