#include "game/scenes/invasion/systems/hud_rendering.hpp"
#include "framework/constants.hpp"
#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/singleton/hud_stats.hpp"
#include "game/scenes/invasion/constants.hpp"
#include "game/util/text_renderer.hpp"
#include <format>

namespace systems {

HUDRendering::HUDRendering(framework::Renderer &renderer)
    : renderer{renderer},
      text_renderer{renderer} {
}

void HUDRendering::execute(framework::ExecuteCtx &ctx) {
  renderer.draw_line(
      framework::DrawLineParams{
          .start_x = 0,
          .start_y = invasion::GROUND_HEIGHT,
          .end_x = framework::WINDOW_WIDTH,
          .end_y = invasion::GROUND_HEIGHT,
          .r = 0,
          .g = 255,
          .b = 0,
      }
  );

  auto hud_stats = ctx.ecs.ctx().get<components::singleton::HUDStats>();

  text_renderer.render_text(6, 6, std::format("score:{}", hud_stats.score));
  text_renderer.render_text(framework::WINDOW_WIDTH / 2 - 31, 6, std::format("hi-score:{}", hud_stats.high_score));
  text_renderer.render_text(framework::WINDOW_WIDTH - 48, 6, std::format("lives:{}", hud_stats.lives));

  if (hud_stats.game_over) {
    text_renderer.render_text(framework::WINDOW_WIDTH / 2 - 28, framework::WINDOW_HEIGHT / 2, "game over");
  }
}

} // namespace systems