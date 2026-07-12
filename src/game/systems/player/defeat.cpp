#include "game/systems/player/defeat.hpp"
#include "core/point.hpp"
#include "framework/ecs/component_manager.hpp"
#include "framework/ecs/ecs.hpp"
#include "framework/ecs/entity.hpp"
#include "framework/ecs/system.hpp"
#include "game/components/deletable.hpp"
#include "game/components/hitpoints.hpp"
#include "game/components/player/movement.hpp"
#include "game/components/position.hpp"
#include "game/components/singleton/hud_stats.hpp"
#include "game/constants.hpp"
#include "game/events/collision_occurred.hpp"
#include "game/events/pause.hpp"
#include <functional>
#include <set>

namespace systems::player {

Defeat::Defeat(
    std::function<void(ecs::ECS &, core::Point, unsigned int)> add_explosion, std::function<void(ecs::ECS &)> add_player
)
    : add_explosion{add_explosion},
      add_player{add_player} {
}

void Defeat::remove_entity(ecs::Entity entity) {
  player_entities.erase(entity);
  deletable_entities.erase(entity);
}

void Defeat::add_entity_if_matches(ecs::Entity entity, ecs::ComponentManager &components) {
  if (components.has<components::player::Movement>(entity) && components.has<components::Hitpoints>(entity)
      && components.has<components::Position>(entity) && components.has<components::Deleteable>(entity)) {
    player_entities.insert(entity);
  }

  if (components.has<components::Deleteable>(entity)) {
    deletable_entities.insert(entity);
  }
}

void Defeat::execute(ecs::ECS &ecs) {
  if (defeat_pause_ongoing) {
    handle_ongoing_pause(ecs);
  } else {
    handle_defeat_if_any(ecs);
  }
}

void Defeat::handle_defeat_if_any(ecs::ECS &ecs) {
  for (const auto &event : ecs.events().get_all<events::CollisionOccurred>()) {
    if (player_entities.contains(event.who_i_hit)
        && ecs.components().get<components::Hitpoints>(event.who_i_hit).cur_hitpoints < 1) {
      auto position = ecs.components().get<components::Position>(event.who_i_hit);

      add_explosion(ecs, {position.x, position.y}, DEFEAT_PAUSE_TICKS);

      ecs.events().set_singleton(
          events::Pause{
              .is_paused = true,
          }
      );

      ecs.components().set(event.who_i_hit, components::Deleteable{.is_deleted = true});

      auto stats = ecs.components().get_singleton<components::singleton::HUDStats>();
      stats.lives -= 1;

      if (stats.lives < 0) {
        stats.game_over = true;
        stats.lives = 0;
        game_is_over = true;
      }

      ecs.components().set_singleton(stats);

      defeat_pause_ongoing = true;
    }
  }
}

void Defeat::handle_ongoing_pause(ecs::ECS &ecs) {
  defeat_pause_counter++;
  if (defeat_pause_counter < DEFEAT_PAUSE_TICKS) {
    return;
  }

  defeat_pause_counter = 0;
  defeat_pause_ongoing = false;

  ecs.events().set_singleton(
      events::Pause{
          .is_paused = false,
      }
  );

  if (game_is_over) {
    game_is_over = false;

    auto stats = ecs.components().get_singleton<components::singleton::HUDStats>();
    stats.score = 0;
    stats.lives = gallia::STARTING_LIVES;
    stats.game_over = false;
    ecs.components().set_singleton(stats);

    delete_all_entities(ecs);
  }

  add_player(ecs);
}

void Defeat::delete_all_entities(ecs::ECS &ecs) {
  for (const auto &entity : deletable_entities) {
    ecs.components().set(
        entity,
        components::Deleteable{
            .is_deleted = true,
        }
    );
  }
}

} // namespace systems::player