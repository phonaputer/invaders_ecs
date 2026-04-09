#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include <memory>

namespace ecs {

DefaultECS::DefaultECS(
    std::vector<std::unique_ptr<ecs::System>> systems, std::unique_ptr<ComponentManager> component_manager
)
    : systems{std::move(systems)},
      component_manager{std::move(component_manager)} {
}

Entity DefaultECS::new_entity() {
  return ++cur_entity_id;
}

ComponentManager &DefaultECS::components() {
  return *component_manager;
}

void DefaultECS::register_to_systems(Entity entity) {
  for (auto &system : systems) {
    if (system->matches_entity(entity, *component_manager)) {
      system->add_entity(entity);
    }
  }
}

void DefaultECS::reregister_to_systems(Entity entity) {
  for (auto &system : systems) {
    system->remove_entity(entity);

    if (system->matches_entity(entity, *component_manager)) {
      system->add_entity(entity);
    }
  }
}

void DefaultECS::delete_from_systems(Entity entity) {
  for (auto &system : systems) {
    system->remove_entity(entity);
  }
}

} // namespace ecs