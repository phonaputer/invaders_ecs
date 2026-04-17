#include "gallia/components/player_movement.hpp"
#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/constants.hpp"
#include "framework/game/player_input.hpp"
#include "framework/game/player_input_manager.hpp"
#include "gallia/components/position.hpp"
#include "gallia/systems/player_movement.hpp"

namespace systems {

PlayerMovement::PlayerMovement(
    game::PlayerInputManager &player_input_manager,
    std::function<ecs::Entity(ecs::ECS &ecs, core::Point)> add_projectile
)
    : player_input_manager{player_input_manager},
      add_projectile{add_projectile} {
}

void PlayerMovement::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void PlayerMovement::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::PlayerMovement>(entity) && components.has<components::Position>(entity)) {
    entities.insert(entity);
  }
}

void PlayerMovement::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto position = ecs.components().get<components::Position>(entity);
    auto movement = ecs.components().get<components::PlayerMovement>(entity);

    if (player_input_manager.is_engaged(game::PlayerInput::LEFT)
        && player_input_manager.is_engaged(game::PlayerInput::RIGHT)) {
      return;
    }

    if (player_input_manager.is_engaged(game::PlayerInput::LEFT)) {
      auto new_x = position.x - movement.x_speed;
      if (new_x < 0) {
        new_x = 0;
      }

      position.x = new_x;

      ecs.components().set<components::Position>(entity, position);
    }

    if (player_input_manager.is_engaged(game::PlayerInput::RIGHT)) {
      auto new_x = position.x + movement.x_speed;
      if (new_x + position.w > game::WINDOW_WIDTH) {
        new_x = game::WINDOW_WIDTH - position.w;
      }

      position.x = new_x;

      ecs.components().set<components::Position>(entity, position);
    }

    std::erase_if(active_projectiles, [&ecs](ecs::Entity projectile) {
      return !ecs.components().has<components::Position>(projectile);
    });

    if (player_input_manager.is_engaged(game::PlayerInput::FIRE)
        && active_projectiles.size() < movement.max_simultaneous_shots
        && movement.shot_clock >= movement.ticks_per_shot) {
      active_projectiles.insert(add_projectile(
          ecs,
          core::Point{
              .x = position.x + movement.shot_offset_x,
              .y = position.y - movement.shot_offset_y,
          }
      ));

      movement.shot_clock = 0;
      ecs.components().set(entity, movement);
    } else {
      movement.shot_clock++;
      ecs.components().set(entity, movement);
    }
  }
}

} // namespace systems