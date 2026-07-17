#include "game/scenes/invasion/systems/score.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/singleton/hud_stats.hpp"
#include "game/scenes/invasion/events/player_scored.hpp"

namespace systems {

void Score::execute(framework::ExecuteCtx &ctx) {
  unsigned int total_score_increment = 0;
  for (const auto &event : ctx.events.get_all<events::PlayerScored>()) {
    total_score_increment += event.score;
  }

  auto stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();

  stats.score += total_score_increment;
  if (stats.score > stats.high_score) {
    stats.high_score = stats.score;
  }

  ctx.ecs.ctx().insert_or_assign(stats);
}

} // namespace systems