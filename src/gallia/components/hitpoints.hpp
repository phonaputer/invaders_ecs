#pragma once

#include "gallia/components/damage_type_enum.hpp"

namespace components {

struct Hitpoints {
    DamageType susceptible_to;
    int cur_hitpoints;
};

} // namespace components