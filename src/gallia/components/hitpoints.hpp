#pragma once

#include "gallia/components/damage_type_enum.hpp"

namespace components {

struct Hitpoints {
    DamageTypeSet susceptible_to;
    int cur_hitpoints;
};

} // namespace components