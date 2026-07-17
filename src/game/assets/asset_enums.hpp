#pragma once

#include <string>

namespace assets {

enum class Image {
  InvadersSpritesheet,
};

inline std::string image_src_id(Image image) {
  switch (image) {
    case Image::InvadersSpritesheet:
      return "invaders_spritesheet";
    default:
      return "unknown";
  }
}

enum class Audio {
  AlienExplosion,
  AlienShot,
  Arp1,
  Arp2,
  Arp3,
  Arp4,
  MenuSelect,
  PlayerExplosion,
  PlayerShot,
};

inline std::string audio_sound_id(Audio audio) {
  switch (audio) {
    case Audio::AlienExplosion:
      return "alien_explosion";
    case Audio::AlienShot:
      return "alien_shot";
    case Audio::Arp1:
      return "arp1";
    case Audio::Arp2:
      return "arp2";
    case Audio::Arp3:
      return "arp3";
    case Audio::Arp4:
      return "arp4";
    case Audio::MenuSelect:
      return "menu_select";
    case Audio::PlayerExplosion:
      return "player_explosion";
    case Audio::PlayerShot:
      return "player_shot";
    default:
      return "unknown";
  }
}

} // namespace assets