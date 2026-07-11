#include "gallia/systems/explode_on_defeat.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/explode_on_defeat.hpp"
#include "gallia/components/hitpoints.hpp"
#include "gallia/components/position.hpp"
#include "gallia/events/collision_occurred.hpp"
#include <set>

namespace systems {

ExplodeOnDefeat::ExplodeOnDefeat(std::function<void(ecs::ECS &, core::Point)> add_explosion)
    : add_explosion{add_explosion} {
}

void ExplodeOnDefeat::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void ExplodeOnDefeat::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::ExplodeOnDefeat>(entity) && components.has<components::Position>(entity)
      && components.has<components::Hitpoints>(entity)) {
    entities.insert(entity);
  }
}

void ExplodeOnDefeat::execute(ecs::ECS &ecs) {
  // iterating collisions because most ticks there won't be a collision but
  // there will always be entities in the entity set
  for (const auto &event : ecs.events().get_all<events::CollisionOccurred>()) {
    if (entities.contains(event.who_i_hit)
        && ecs.components().get<components::Hitpoints>(event.who_i_hit).cur_hitpoints < 1) {
      auto position = ecs.components().get<components::Position>(event.who_i_hit);

      add_explosion(ecs, {position.x, position.y});
    }
  }
}

} // namespace systems