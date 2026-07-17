#include "game/scenes/invasion/hud.hpp"
#include "game/scenes/invasion/components/singleton/hud_stats.hpp"
#include "game/scenes/invasion/constants.hpp"
#include <entt.hpp>

namespace invasion {

void add_hud_entity(entt::registry &ecs) {
  ecs.ctx().emplace<components::singleton::HUDStats>(components::singleton::HUDStats{
      .lives = invasion::STARTING_LIVES,
      .score = 0,
      .high_score = 0,
      .game_over = false,
  });
}

} // namespace invasion