#pragma once

#include "gallia/components/damage_type_enum.hpp"

namespace components {

struct DamageDealer {
    DamageType type;
    int amount;
};

} // namespace components