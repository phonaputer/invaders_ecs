#include "game/scenes/invasion/systems/debug/hitbox_rendering.hpp"
#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/collision.hpp"
#include "game/scenes/invasion/components/position.hpp"

namespace systems::debug {

HitboxRendering::HitboxRendering(framework::Renderer &renderer)
    : renderer{renderer} {
}

void HitboxRendering::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<const components::Collision, const components::Position>();

  for (auto [entity, collision, position] : view.each()) {
    renderer.draw_rect(
        framework::DrawRectParams{
            .x = position.x + collision.hitbox_offset_x,
            .y = position.y + collision.hitbox_offset_y,
            .width = collision.hitbox_w,
            .height = collision.hitbox_h,
            .r = 255,
            .g = 0,
            .b = 255,
        }
    );
  }
}

} // namespace systems::debug