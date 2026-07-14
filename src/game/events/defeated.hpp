#pragma once

#include "framework/ecs/entity.hpp"

namespace events {

struct Defeated {
    ecs::Entity entity;
};

} // namespace events