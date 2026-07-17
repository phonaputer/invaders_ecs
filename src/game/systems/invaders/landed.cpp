#include "game/systems/invaders/landed.hpp"
#include "game/components/collision.hpp"
#include "game/components/invader.hpp"
#include "game/components/position.hpp"
#include "game/constants.hpp"
#include "game/events/aliens_landed.hpp"

namespace systems::invaders {

void Landed::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::Invader, components::Position, components::Collision>();

  for (auto [entity, position, collision] : view.each()) {
    if (position.y + collision.hitbox_offset_y + collision.hitbox_h >= game::GROUND_HEIGHT) {
      ctx.events.set_singleton(events::AliensLanded{});
    }
  }
}

} // namespace systems::invaders