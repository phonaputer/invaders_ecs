#include "game/scenes/invasion/systems/audio.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/events/play_audio.hpp"
#include "game/scenes/invasion/events/stop_audio.hpp"

namespace systems {

Audio::Audio(framework::AudioPlayer &audio_player)
    : audio_player{audio_player} {
}

void Audio::execute(framework::ExecuteCtx &ctx) {
  for (const auto &event : ctx.events.get_all_draw<events::PlayAudio>()) {
    sounds_to_play.insert(event.audio);
  }

  for (const auto &event : ctx.events.get_all_draw<events::StopAudio>()) {
    sounds_to_play.erase(event.audio);
    audio_player.stop_sound(assets::audio_sound_id(event.audio));
  }

  for (const auto &sound : sounds_to_play) {
    audio_player.play_sound(assets::audio_sound_id(sound));
  }

  sounds_to_play.clear();
}

} // namespace systems