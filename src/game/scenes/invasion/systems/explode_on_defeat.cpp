#include "game/scenes/invasion/systems/explode_on_defeat.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/explode_on_defeat.hpp"
#include "game/scenes/invasion/components/position.hpp"
#include "game/scenes/invasion/events/defeated.hpp"
#include <entt.hpp>

namespace systems {

ExplodeOnDefeat::ExplodeOnDefeat(std::function<void(entt::registry &, core::Point)> add_explosion)
    : add_explosion{add_explosion} {
}

void ExplodeOnDefeat::execute(framework::ExecuteCtx &ctx) {
  for (const auto &event : ctx.events.get_all<events::Defeated>()) {
    if (ctx.ecs.all_of<components::ExplodeOnDefeat, components::Position>(event.entity)) {
      auto position = ctx.ecs.get<components::Position>(event.entity);

      add_explosion(ctx.ecs, {position.x, position.y});
    }
  }
}

} // namespace systems