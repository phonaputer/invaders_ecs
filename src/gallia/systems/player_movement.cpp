#include "gallia/components/player_movement.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/constants.hpp"
#include "framework/game/player_input.hpp"
#include "framework/game/player_input_manager.hpp"
#include "gallia/components/animation.hpp"
#include "gallia/components/position.hpp"
#include "gallia/systems/player_movement.hpp"

namespace systems {

PlayerMovement::PlayerMovement(game::PlayerInputManager &player_input_manager)
    : player_input_manager{player_input_manager} {
}

void PlayerMovement::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void PlayerMovement::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::PlayerMovement>(entity) && components.has<components::Position>(entity)
      && components.has<components::Animation>(entity)) {
    entities.insert(entity);
  }
}

void PlayerMovement::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto position = ecs.components().get<components::Position>(entity);
    auto movement = ecs.components().get<components::PlayerMovement>(entity);
    auto animation = ecs.components().get<components::Animation>(entity);

    if (player_input_manager.is_engaged(game::PlayerInput::LEFT)
        == player_input_manager.is_engaged(game::PlayerInput::RIGHT)) {
      animation.playing = false;
      ecs.components().set(entity, animation);
      return;
    }

    if (player_input_manager.is_engaged(game::PlayerInput::LEFT)) {
      auto new_x = position.x - movement.x_speed;
      if (new_x < 0) {
        new_x = 0;
      }

      if (new_x != position.x) {
        animation.playing = true;
        animation.play_reversed = true;
      } else {
        animation.playing = false;
      }
      ecs.components().set(entity, animation);

      position.x = new_x;

      ecs.components().set<components::Position>(entity, position);
    }

    if (player_input_manager.is_engaged(game::PlayerInput::RIGHT)) {
      auto new_x = position.x + movement.x_speed;
      if (new_x + position.w > game::WINDOW_WIDTH) {
        new_x = game::WINDOW_WIDTH - position.w;
      }

      if (new_x != position.x) {
        animation.playing = true;
        animation.play_reversed = false;
      } else {
        animation.playing = false;
      }
      ecs.components().set(entity, animation);

      position.x = new_x;

      ecs.components().set<components::Position>(entity, position);
    }
  }
}

} // namespace systems