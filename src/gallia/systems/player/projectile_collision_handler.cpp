#include "gallia/systems/player/projectile_collision_handler.hpp"
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

namespace systems::player {

void ProjectileCollisionHandler::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void ProjectileCollisionHandler::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Deleteable>(entity) && components.has<components::Collision>(entity)
      && components.has<components::player::IsProjectile>(entity)) {
    entities.insert(entity);
  }
}

void ProjectileCollisionHandler::execute(ecs::ECS &ecs) {
  auto collisions = ecs.messages().get_all<messages::CollisionOccurred>();

  for (const auto &collision : collisions) {
    if (!entities.contains(collision.who_am_i)) {
      continue;
    }

    if (!ecs.components().has<components::invaders::IsInvader>(collision.who_i_hit)) {
      continue;
    }

    auto deletion = ecs.components().get<components::Deleteable>(collision.who_am_i);
    deletion.is_deleted = true;
    ecs.components().set(collision.who_am_i, deletion);
  }
}

} // namespace systems::player