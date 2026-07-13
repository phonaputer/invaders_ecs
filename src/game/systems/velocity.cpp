#include "game/systems/velocity.hpp"
#include "framework/constants.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "game/components/deletable.hpp"
#include "game/components/position.hpp"
#include "game/components/velocity.hpp"
#include "game/events/pause.hpp"
#include <set>

namespace systems {

void Velocity::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void Velocity::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::Velocity>(entity) && components.has<components::Position>(entity)) {
    entities.insert(entity);
  }
}

void Velocity::execute(ecs::ECS &ecs) {
  const auto pause = ecs.events().get_singleton<events::Pause>();
  if (pause.has_value()) {
    paused = pause.value().is_paused;
  }

  if (paused) {
    return;
  }

  for (const auto &entity : entities) {
    auto position = ecs.components().get<components::Position>(entity);
    auto velocity = ecs.components().get<components::Velocity>(entity);

    position.x += velocity.x;
    position.y += velocity.y;

    ecs.components().set<components::Position>(entity, position);

    if (ecs.components().has<components::Deleteable>(entity)) {
      if (position.x + position.w < 0 || position.x > framework::WINDOW_WIDTH || position.y + position.h < 0
          || position.y > framework::WINDOW_HEIGHT) {
        ecs.components().set(entity, components::Deleteable{.is_deleted = true});
      }
    }
  }
}

} // namespace systems