#pragma once

#include <bitset>

namespace components {

enum class DamageType { Alien, Player, Fortress, Alien_Projectile, Player_Projectile, COUNT };

inline size_t damage_type_to_index(DamageType dt) {
  return static_cast<size_t>(dt);
}

using DamageTypeSet = std::bitset<static_cast<size_t>(DamageType::COUNT)>;

} // namespace components