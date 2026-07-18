#include "game/scenes/invasion/systems/velocity.hpp"
#include "framework/constants.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/deletable.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/components/singleton/paused.hpp"
#include "game/scenes/invasion/components/velocity.hpp"

namespace systems {

void Velocity::execute(framework::ExecuteCtx &ctx) {
  const auto pause = ctx.ecs.ctx().get<components::singleton::Paused>();
  if (pause.paused) {
    return;
  }

  auto view = ctx.ecs.view<components::Position, components::Velocity>();

  for (auto [entity, position, velocity] : view.each()) {
    position.x += velocity.x;
    position.y += velocity.y;

    if (position.x + position.w < 0 || position.x > framework::WINDOW_WIDTH || position.y + position.h < 0
        || position.y > framework::WINDOW_HEIGHT) {
      ctx.ecs.emplace<components::Deleteable>(entity);
    }
  }
}

} // namespace systems