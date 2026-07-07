#include "gallia/systems/invaders/collision_handler.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/invaders/enabled.hpp"
#include <set>

namespace systems::invaders {

void CollisionHandler::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void CollisionHandler::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::invaders::Enabled>(entity) && components.has<components::Collision>(entity)) {
    entities.insert(entity);
  }
}

void CollisionHandler::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
  }
}

} // namespace systems::invaders