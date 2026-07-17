#include "game/scenes/invasion/systems/invaders/landed.hpp"
#include "game/scenes/invasion/components/collision.hpp"
#include "game/scenes/invasion/components/invader.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/constants.hpp"
#include "game/scenes/invasion/events/aliens_landed.hpp"

namespace systems::invaders {

void Landed::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::Invader, components::Position, components::Collision>();

  for (auto [entity, position, collision] : view.each()) {
    if (position.y + collision.hitbox_offset_y + collision.hitbox_h >= invasion::GROUND_HEIGHT) {
      ctx.events.set_singleton(events::AliensLanded{});
    }
  }
}

} // namespace systems::invaders