#include "gallia/systems/damage.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/damage_dealer.hpp"
#include "gallia/components/damage_type_enum.hpp"
#include "gallia/components/deletable.hpp"
#include "gallia/components/hitpoints.hpp"
#include "gallia/messages/collision_occurred.hpp"
#include <set>

namespace systems {

void Damage::remove_entity(ecs::Entity entity) {
  dealers.erase(entity);
  receivers.erase(entity);
}

void Damage::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::DamageDealer>(entity)) {
    dealers.insert(entity);
  }

  if (components.has<components::Hitpoints>(entity)) {
    receivers.insert(entity);
  }
}

void Damage::execute(ecs::ECS &ecs) {
  auto collisions = ecs.messages().get_all<messages::CollisionOccurred>();

  for (const auto &collision : collisions) {
    if (!dealers.contains(collision.who_am_i)) {
      continue;
    }

    if (!receivers.contains(collision.who_i_hit)) {
      continue;
    }

    auto damage = ecs.components().get<components::DamageDealer>(collision.who_am_i);
    auto hitpoints = ecs.components().get<components::Hitpoints>(collision.who_i_hit);

    if (damage.type != hitpoints.susceptible_to) {
      continue;
    }

    hitpoints.cur_hitpoints -= damage.amount;
    ecs.components().set(collision.who_i_hit, hitpoints);

    if (hitpoints.cur_hitpoints > 0) {
      continue;
    }

    if (ecs.components().has<components::Deleteable>(collision.who_i_hit)) {
      ecs.components().set(
          collision.who_i_hit,
          components::Deleteable{
              .is_deleted = true,
          }
      );
    }
  }
}

} // namespace systems