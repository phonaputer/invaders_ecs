#include "game/systems/audio.hpp"
#include "framework/system.hpp"
#include "game/events/play_audio.hpp"
#include "game/events/stop_audio.hpp"

namespace systems {

Audio::Audio(framework::AudioPlayer &audio_player)
    : audio_player{audio_player} {
}

void Audio::execute(framework::ExecuteCtx &ctx) {
  for (const auto &event : ctx.events.get_all<events::PlayAudio>()) {
    sounds_to_play.insert(event.audio_id);
  }

  for (const auto &event : ctx.events.get_all<events::StopAudio>()) {
    sounds_to_play.erase(event.audio_id);
    audio_player.stop_sound(event.audio_id);
  }

  for (const auto &sound : sounds_to_play) {
    audio_player.play_sound(sound);
  }

  sounds_to_play.clear();
}

} // namespace systems