#include "gallia/systems/deletion.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/deletable.hpp"
#include <set>

namespace systems {

void Deletion::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void Deletion::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Deleteable>(entity)) {
    entities.insert(entity);
  }
}

void Deletion::execute(ecs::ECS &ecs) {
  std::set<ecs::Entity> cloned_entities(entities);

  for (const auto &entity : cloned_entities) {
    auto deleteable = ecs.components().get<components::Deleteable>(entity);

    if (deleteable.is_deleted) {
      ecs.delete_entity(entity);
    }
  }
}

} // namespace systems