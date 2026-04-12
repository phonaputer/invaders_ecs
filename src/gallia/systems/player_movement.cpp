#include "gallia/systems/player_movement.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/constants.hpp"
#include "framework/game/player_input.hpp"
#include "framework/game/player_input_manager.hpp"
#include "framework/game/renderer.hpp"
#include "gallia/components/player_movement.hpp"
#include "gallia/components/position.hpp"

namespace systems {

PlayerMovement::PlayerMovement(game::PlayerInputManager &player_input_manager)
    : player_input_manager{player_input_manager} {
}

void PlayerMovement::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void PlayerMovement::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::PlayerMovement>(entity) && components.has<components::Position>(entity)) {
    entities.insert(entity);
  }
}

void PlayerMovement::execute(ecs::ComponentManager &components) {
  for (const auto &entity : entities) {
    auto position = components.get<components::Position>(entity);
    auto movement = components.get<components::PlayerMovement>(entity);

    if (player_input_manager.is_engaged(game::PlayerInput::LEFT) &&
        player_input_manager.is_engaged(game::PlayerInput::RIGHT)) {
      return;
    }

    if (player_input_manager.is_engaged(game::PlayerInput::LEFT)) {
      auto new_x = position.x - movement.x_speed;
      if (new_x < 0) {
        new_x = 0;
      }

      position.x = new_x;

      components.set<components::Position>(entity, position);
    }

    if (player_input_manager.is_engaged(game::PlayerInput::RIGHT)) {
      auto new_x = position.x + movement.x_speed;
      if (new_x + movement.width > game::WINDOW_WIDTH) {
        new_x = game::WINDOW_WIDTH - movement.width;
      }

      position.x = new_x;

      components.set<components::Position>(entity, position);
    }
  }
}

} // namespace systems