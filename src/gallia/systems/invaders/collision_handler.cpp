#include "gallia/systems/invaders/collision_handler.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/deletable.hpp"
#include <set>

namespace systems::invaders {

void CollisionHandler::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void CollisionHandler::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Deleteable>(entity) && components.has<components::Collision>(entity)) {
    entities.insert(entity);
  }
}

// TODO this feels kinda janky to have all this in one place...
void CollisionHandler::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto collision = ecs.components().get<components::Collision>(entity);

    if (!collision.hit_something_this_tick) {
      continue;
    }

    if (collision.type == components::collision::Type::Alien
        && collision.type_of_who_i_hit == components::collision::Type::PlayerProjectile) {

      auto deletion = ecs.components().get<components::Deleteable>(entity);
      deletion.is_deleted = true;
      ecs.components().set(entity, deletion);
    }

    if (collision.type == components::collision::Type::PlayerProjectile
        && collision.type_of_who_i_hit == components::collision::Type::Alien) {

      auto deletion = ecs.components().get<components::Deleteable>(entity);
      deletion.is_deleted = true;
      ecs.components().set(entity, deletion);
    }
  }
}

} // namespace systems::invaders