#include "game/systems/player/shooting.hpp"
#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "framework/player_input.hpp"
#include "framework/player_input_manager.hpp"
#include "game/components/deletable.hpp"
#include "game/components/player/shooting.hpp"
#include "game/components/position.hpp"
#include <functional>
#include <set>

namespace systems::player {

Shooting::Shooting(
    game::PlayerInputManager &player_input_manager,
    std::function<ecs::Entity(ecs::ECS &ecs, core::Point)> add_projectile,
    std::function<void(ecs::ECS &, ecs::Entity)> add_muzzle_flash
)
    : player_input_manager{player_input_manager},
      add_projectile{add_projectile},
      add_muzzle_flash{add_muzzle_flash} {
}

void Shooting::remove_entity(ecs::Entity entity) {
  entities.erase(entity);
}

void Shooting::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::player::Shooting>(entity) && components.has<components::Position>(entity)) {
    entities.insert(entity);
  }
}

void Shooting::execute(ecs::ECS &ecs) {
  for (const auto &entity : entities) {
    auto position = ecs.components().get<components::Position>(entity);
    auto shooting = ecs.components().get<components::player::Shooting>(entity);

    std::erase_if(active_projectiles, [&ecs](ecs::Entity projectile) {
      return !ecs.components().has<components::Position>(projectile);
    });

    bool fire = false;

    if (player_input_manager.is_engaged(game::PlayerInput::FIRE)
        && active_projectiles.size() < shooting.max_simultaneous_shots
        && shooting.shot_clock >= shooting.ticks_per_shot) {
      fire = true;
      shooting.shot_clock = 0;
    } else {
      shooting.shot_clock++;
    }

    ecs.components().set(entity, shooting);

    if (fire) {
      active_projectiles.insert(add_projectile(
          ecs,
          core::Point{
              .x = position.x + shooting.shot_offset_x,
              .y = position.y - shooting.shot_offset_y,
          }
      ));
      add_muzzle_flash(ecs, entity);
    }
  }
}

} // namespace systems::player