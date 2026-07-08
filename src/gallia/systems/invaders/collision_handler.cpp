#include "gallia/systems/invaders/collision_handler.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/deletable.hpp"
#include "gallia/components/invaders/is_invader.hpp"
#include "gallia/components/player/is_projectile.hpp"
#include "gallia/messages/collision_occurred.hpp"
#include <set>

namespace systems::invaders {

void CollisionHandler::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void CollisionHandler::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Deleteable>(entity) && components.has<components::Collision>(entity)
      && components.has<components::invaders::IsInvader>(entity)) {
    entities.insert(entity);
  }
}

void CollisionHandler::execute(ecs::ECS &ecs) {
  auto collisions = ecs.messages().get_all<messages::CollisionOccurred>();

  for (const auto &collision : collisions) {
    if (!entities.contains(collision.who_am_i)) {
      continue;
    }

    if (!ecs.components().has<components::player::IsProjectile>(collision.who_i_hit)) {
      continue;
    }

    auto deletion = ecs.components().get<components::Deleteable>(collision.who_am_i);
    deletion.is_deleted = true;
    ecs.components().set(collision.who_am_i, deletion);
  }
}

} // namespace systems::invaders