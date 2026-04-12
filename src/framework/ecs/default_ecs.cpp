#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include <memory>

namespace ecs {

DefaultECS::DefaultECS(std::unique_ptr<ComponentManager> component_manager)
    : component_manager{std::move(component_manager)} {
}

Entity DefaultECS::new_entity() {
  return ++cur_entity_id;
}

ComponentManager &DefaultECS::components() {
  return *component_manager;
}

void DefaultECS::add_update_system(std::unique_ptr<System> system) {
  update_systems.push_back(std::move(system));
}

void DefaultECS::add_draw_system(std::unique_ptr<System> system) {
  draw_systems.push_back(std::move(system));
}

void DefaultECS::register_to_systems(Entity entity) {
  for (auto &system : update_systems) {
    system->remove_entity(entity);
    system->add_entity_if_matches(entity, *component_manager);
  }

  for (auto &system : draw_systems) {
    system->remove_entity(entity);
    system->add_entity_if_matches(entity, *component_manager);
  }
}

void DefaultECS::delete_from_systems(Entity entity) {
  for (auto &system : update_systems) {
    system->remove_entity(entity);
  }

  for (auto &system : draw_systems) {
    system->remove_entity(entity);
  }
}

void DefaultECS::update() {
  for (auto &system : update_systems) {
    system->execute(*component_manager);
  }
}

void DefaultECS::draw() {
  for (auto &system : draw_systems) {
    system->execute(*component_manager);
  }
}

} // namespace ecs