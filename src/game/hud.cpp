#include "game/hud.hpp"
#include "framework/ecs/ecs.hpp"
#include "game/components/singleton/hud_stats.hpp"
#include "game/constants.hpp"

namespace gallia {

void add_hud_entity(ecs::ECS &ecs) {
  ecs.components().set_singleton(
      components::singleton::HUDStats{
          .lives = gallia::STARTING_LIVES,
          .score = 0,
          .high_score = 0,
          .game_over = false,
      }
  );
}

} // namespace gallia