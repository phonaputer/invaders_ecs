#include "game/scenes/invasion/systems/sound_on_defeat.hpp"
#include "framework/system.hpp"
#include "game/scenes/invasion/components/sound_on_defeat.hpp"
#include "game/scenes/invasion/events/defeated.hpp"
#include "game/scenes/invasion/events/play_audio.hpp"

namespace systems {

void SoundOnDefeat::execute(framework::ExecuteCtx &ctx) {
  for (const auto &event : ctx.events.get_all<events::Defeated>()) {
    if (ctx.ecs.all_of<components::SoundOnDefeat>(event.entity)) {
      ctx.events.push_back_draw(
          events::PlayAudio{
              .audio = ctx.ecs.get<components::SoundOnDefeat>(event.entity).audio,
          }
      );
    }
  }
}

} // namespace systems