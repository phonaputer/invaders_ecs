#pragma once

#include "game/scenes/invasion/components/damage_type_enum.hpp"

namespace components {

struct DamageDealer {
    DamageTypeSet type;
    int amount;
};

} // namespace components