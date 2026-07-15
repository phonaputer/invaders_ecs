#pragma once

#include <entt.hpp>

namespace events {

struct TookDamage {
    entt::entity entity;
    int amount;
};

} // namespace events