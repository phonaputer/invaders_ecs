#include "game/systems/rendering.hpp"
#include "framework/renderer.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/components/position.hpp"
#include "game/components/sprite.hpp"

namespace systems {

Rendering::Rendering(framework::Renderer &renderer)
    : renderer{renderer} {
}

void Rendering::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::Position, components::Sprite>();
  view.use<components::Position>();

  for (auto [entity, position, sprite] : view.each()) {
    renderer.draw_image(
        framework::DrawImageParams{
            .src_id = assets::image_src_id(sprite.image),
            .src_x = sprite.src_x,
            .src_y = sprite.src_y,
            .src_width = sprite.src_width,
            .src_height = sprite.src_height,
            .dst_x = position.x,
            .dst_y = position.y,
            .dst_width = sprite.dst_width,
            .dst_height = sprite.dst_height,
        }
    );
  }
}

} // namespace systems