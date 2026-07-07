#include "gallia/systems/player/projectile_collision_handler.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "gallia/components/collision.hpp"
#include "gallia/components/deletable.hpp"
#include "gallia/components/invaders/is_invader.hpp"
#include "gallia/components/player/is_projectile.hpp"
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
  for (const auto &entity : entities) {
    auto collision = ecs.components().get<components::Collision>(entity);

    if (!collision.hit_something_this_tick) {
      continue;
    }

    if (ecs.components().has<components::invaders::IsInvader>(collision.who_i_hit)) {
      auto deletion = ecs.components().get<components::Deleteable>(entity);
      deletion.is_deleted = true;
      ecs.components().set(entity, deletion);
    }
  }
}

} // namespace systems::player