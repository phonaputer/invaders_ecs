#include "game/systems/explode_on_defeat.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "game/components/explode_on_defeat.hpp"
#include "game/components/position.hpp"
#include "game/events/defeated.hpp"
#include <set>

namespace systems {

ExplodeOnDefeat::ExplodeOnDefeat(std::function<void(ecs::ECS &, core::Point)> add_explosion)
    : add_explosion{add_explosion} {
}

void ExplodeOnDefeat::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void ExplodeOnDefeat::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::ExplodeOnDefeat>(entity) && components.has<components::Position>(entity)) {
    entities.insert(entity);
  }
}

void ExplodeOnDefeat::execute(ecs::ECS &ecs) {
  for (const auto &event : ecs.events().get_all<events::Defeated>()) {
    if (entities.contains(event.entity)) {
      auto position = ecs.components().get<components::Position>(event.entity);

      add_explosion(ecs, {position.x, position.y});
    }
  }
}

} // namespace systems