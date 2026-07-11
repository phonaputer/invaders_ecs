#include "game/systems/position_following.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "game/components/deletable.hpp"
#include "game/components/position.hpp"
#include "game/components/position_following.hpp"

namespace systems {

void PositionFollowing::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void PositionFollowing::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::PositionFollowing>(entity) && components.has<components::Position>(entity)
      && components.has<components::Deleteable>(entity)) {
    entities.insert(entity);
  }
}

void PositionFollowing::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto following = ecs.components().get<components::PositionFollowing>(entity);

    if (!ecs.components().has<components::Position>(following.leader)) {
      ecs.components().set(entity, components::Deleteable{.is_deleted = true});
      continue;
    }

    auto leader_position = ecs.components().get<components::Position>(following.leader);

    ecs.components().set(entity, leader_position);
  }
}

} // namespace systems