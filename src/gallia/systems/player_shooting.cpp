#include "gallia/components/player_shooting.hpp"
#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/game/constants.hpp"
#include "framework/game/player_input.hpp"
#include "framework/game/player_input_manager.hpp"
#include "gallia/components/position.hpp"
#include "gallia/systems/player_shooting.hpp"

namespace systems {

PlayerShooting::PlayerShooting(
    game::PlayerInputManager &player_input_manager,
    std::function<ecs::Entity(ecs::ECS &ecs, core::Point)> add_projectile
)
    : player_input_manager{player_input_manager},
      add_projectile{add_projectile} {
}

void PlayerShooting::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void PlayerShooting::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::PlayerShooting>(entity) && components.has<components::Position>(entity)) {
    entities.insert(entity);
  }
}

void PlayerShooting::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto position = ecs.components().get<components::Position>(entity);
    auto shooting = ecs.components().get<components::PlayerShooting>(entity);

    std::erase_if(active_projectiles, [&ecs](ecs::Entity projectile) {
      return !ecs.components().has<components::Position>(projectile);
    });

    if (player_input_manager.is_engaged(game::PlayerInput::FIRE)
        && active_projectiles.size() < shooting.max_simultaneous_shots
        && shooting.shot_clock >= shooting.ticks_per_shot) {
      active_projectiles.insert(add_projectile(
          ecs,
          core::Point{
              .x = position.x + shooting.shot_offset_x,
              .y = position.y - shooting.shot_offset_y,
          }
      ));

      shooting.shot_clock = 0;
      ecs.components().set(entity, shooting);
    } else {
      shooting.shot_clock++;
      ecs.components().set(entity, shooting);
    }
  }
}

} // namespace systems