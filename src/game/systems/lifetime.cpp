#include "game/systems/lifetime.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "game/components/deletable.hpp"
#include "game/components/lifetime.hpp"

namespace systems {

void Lifetime::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void Lifetime::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Lifetime>(entity) && components.has<components::Deleteable>(entity)) {
    entities.insert(entity);
  }
}

void Lifetime::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto lifetime = ecs.components().get<components::Lifetime>(entity);

    lifetime.tick_counter++;
    ecs.components().set(entity, lifetime);

    if (lifetime.tick_counter >= lifetime.ticks) {
      ecs.components().set(entity, components::Deleteable{.is_deleted = true});
    }
  }
}

} // namespace systems