#include "framework/ecs/default_ecs.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/message_board.hpp"
#include "framework/ecs/system.hpp"
#include <memory>

namespace ecs {

DefaultECS::DefaultECS(std::unique_ptr<ComponentManager> component_manager, std::unique_ptr<MessageBoard> message_board)
    : component_manager{std::move(component_manager)},
      message_board{std::move(message_board)} {
}

Entity DefaultECS::new_entity() {
  return ++cur_entity_id;
}

ComponentManager &DefaultECS::components() {
  return *component_manager;
}

MessageBoard &DefaultECS::messages() {
  return *message_board;
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

void DefaultECS::delete_entity(Entity entity) {
  for (auto &system : update_systems) {
    system->remove_entity(entity);
  }

  for (auto &system : draw_systems) {
    system->remove_entity(entity);
  }

  component_manager->delete_entity(entity);
}

void DefaultECS::update() {
  for (auto &system : update_systems) {
    system->execute(*this);
  }
}

void DefaultECS::draw() {
  for (auto &system : draw_systems) {
    system->execute(*this);
  }
}

} // namespace ecs