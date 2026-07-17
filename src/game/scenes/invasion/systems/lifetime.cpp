#include "game/scenes/invasion/systems/lifetime.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/deletable.hpp"
#include "game/scenes/invasion/components/lifetime.hpp"

namespace systems {

void Lifetime::execute(framework::ExecuteCtx &ctx) {
  auto view = ctx.ecs.view<components::Lifetime>();

  for (auto [entity, lifetime] : view.each()) {
    lifetime.tick_counter++;

    if (lifetime.tick_counter >= lifetime.ticks) {
      ctx.ecs.emplace<components::Deleteable>(entity);
    }
  }
}

} // namespace systems