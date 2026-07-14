#pragma once

#include "framework/ecs/entity.hpp"

namespace events {

struct TookDamage {
    ecs::Entity entity;
    int amount;
};

} // namespace events