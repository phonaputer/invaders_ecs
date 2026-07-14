#include "game/systems/lifetime.hpp"
#include "framework/system.hpp"
#include "game/components/deletable.hpp"
#include "game/components/lifetime.hpp"

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