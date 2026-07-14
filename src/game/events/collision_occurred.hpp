#pragma once

#include <flecs.h>

namespace events {

struct CollisionOccurred {
    flecs::entity who_am_i;
    flecs::entity who_i_hit;

    bool operator==(const CollisionOccurred &) const = default;
};

} // namespace events