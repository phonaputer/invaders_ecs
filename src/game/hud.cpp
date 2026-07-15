#include "game/hud.hpp"
#include "game/components/singleton/hud_stats.hpp"
#include "game/constants.hpp"
#include <entt.hpp>

namespace game {

void add_hud_entity(entt::registry &ecs) {
  ecs.ctx().emplace<components::singleton::HUDStats>(components::singleton::HUDStats{
      .lives = game::STARTING_LIVES,
      .score = 0,
      .high_score = 0,
      .game_over = false,
  });
}

} // namespace game