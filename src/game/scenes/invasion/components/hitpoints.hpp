#pragma once

#include "game/scenes/invasion/components/damage_type_enum.hpp"

namespace components {

struct Hitpoints {
    DamageTypeSet susceptible_to;
    int cur_hitpoints;
    bool grants_score = false;
    unsigned int score_value = 0;
};

} // namespace components