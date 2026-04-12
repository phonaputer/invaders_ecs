#include "gallia/systems/player_movement.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
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

bool PlayerMovement::matches_entity(ecs::Entity entity, ecs::ComponentManager &components) {
  return (components.has<components::PlayerMovement>(entity) && components.has<components::Position>(entity));
}

void PlayerMovement::add_entity(ecs::Entity entity) {
  entities.insert(entity);
}

void PlayerMovement::execute(ecs::ComponentManager &components) {
  for (const auto &entity : entities) {
    auto position = components.get<components::Position>(entity);
    auto movement = components.get<components::PlayerMovement>(entity);

    if (player_input_manager.is_engaged(game::PlayerInput::LEFT)) {
      position.x -= movement.x_speed;
      components.set<components::Position>(entity, position);
    } else if (player_input_manager.is_engaged(game::PlayerInput::RIGHT)) {
      position.x += movement.x_speed;
      components.set<components::Position>(entity, position);
    }
  }
}

} // namespace systems