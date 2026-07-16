#include "game/systems/explode_on_defeat.hpp"
#include "framework/system.hpp"
#include "game/components/explode_on_defeat.hpp"
#include "game/components/position.hpp"
#include "game/events/defeated.hpp"
#include "game/events/play_audio.hpp"
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

      ctx.events.push_back(
          events::PlayAudio{
              .audio_id = "alien_explosion",
          }
      );
    }
  }
}

} // namespace systems