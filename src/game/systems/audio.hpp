#pragma once

#include "framework/audio_player.hpp"
#include "framework/system.hpp"
#include "game/assets/asset_enums.hpp"
#include <set>

namespace systems {

class Audio : public framework::System {
  private:
    framework::AudioPlayer &audio_player;

    std::set<assets::Audio> sounds_to_play;

  public:
    Audio(framework::AudioPlayer &audio_player);
    void execute(framework::ExecuteCtx &ctx) override;
};

} // namespace systems