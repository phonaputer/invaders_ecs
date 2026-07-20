#include "framework/scene.hpp"
#include "game/assets/asset_enums.hpp"
#include "game/scenes/invasion/components/singleton/paused.hpp"
#include "game/scenes/invasion/fortress.hpp"
#include "game/scenes/invasion/hud.hpp"
#include "game/scenes/invasion/invaders.hpp"
#include "game/scenes/invasion/player.hpp"
#include "game/scenes/invasion/scene.hpp"
#include "game/scenes/invasion/systems/animation.hpp"
#include "game/scenes/invasion/systems/audio.hpp"
#include "game/scenes/invasion/systems/collision_detection.hpp"
#include "game/scenes/invasion/systems/damage.hpp"
#include "game/scenes/invasion/systems/debug/hitbox_rendering.hpp"
#include "game/scenes/invasion/systems/deletion.hpp"
#include "game/scenes/invasion/systems/explode_on_defeat.hpp"
#include "game/scenes/invasion/systems/hud_rendering.hpp"
#include "game/scenes/invasion/systems/invaders/eel.hpp"
#include "game/scenes/invasion/systems/invaders/landed.hpp"
#include "game/scenes/invasion/systems/invaders/orchestration.hpp"
#include "game/scenes/invasion/systems/lifetime.hpp"
#include "game/scenes/invasion/systems/player/game_over.hpp"
#include "game/scenes/invasion/systems/player/movement.hpp"
#include "game/scenes/invasion/systems/player/shooting.hpp"
#include "game/scenes/invasion/systems/position_following.hpp"
#include "game/scenes/invasion/systems/rendering.hpp"
#include "game/scenes/invasion/systems/score.hpp"
#include "game/scenes/invasion/systems/sound_on_defeat.hpp"
#include "game/scenes/invasion/systems/sprite_offset_on_damage.hpp"
#include "game/scenes/invasion/systems/velocity.hpp"
#include <random>

namespace invasion {

void Scene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_image_png(
      assets::image_src_id(assets::Image::InvadersSpritesheet), assets::image_path(assets::Image::InvadersSpritesheet)
  );

  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::AlienExplosion), "./assets/alien_explosion.wav");
  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::AlienShot), "./assets/alien_shot.wav");
  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::PlayerExplosion), "./assets/player_explosion.wav");
  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::PlayerShot), "./assets/player_shot.wav");
  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::Arp1), "./assets/arp1.wav");
  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::Arp2), "./assets/arp2.wav");
  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::Arp3), "./assets/arp3.wav");
  ctx.assets.load_audio_wav(assets::audio_sound_id(assets::Audio::Arp4), "./assets/arp4.wav");

  std::random_device rd;

  ctx.systems.add_update_system(std::make_unique<systems::CollisionDetection>());
  ctx.systems.add_update_system(std::make_unique<systems::Damage>());
  ctx.systems.add_update_system(std::make_unique<systems::SpriteOffsetOnDamage>());
  ctx.systems.add_update_system(std::make_unique<systems::invaders::Landed>());
  ctx.systems.add_update_system(
      std::make_unique<systems::player::GameOver>(add_player_explosion_entity, add_player_entity, add_fortresses)
  );
  ctx.systems.add_update_system(
      std::make_unique<systems::invaders::Orchestration>(add_invader_projectile, add_invader_entities, rd())
  );
  ctx.systems.add_update_system(std::make_unique<systems::ExplodeOnDefeat>(add_explosion));
  ctx.systems.add_update_system(std::make_unique<systems::SoundOnDefeat>());
  ctx.systems.add_update_system(std::make_unique<systems::Deletion>());
  ctx.systems.add_update_system(std::make_unique<systems::player::Movement>());
  ctx.systems.add_update_system(
      std::make_unique<systems::player::Shooting>(add_player_projectile_entity, add_player_muzzle_flash_entity)
  );
  ctx.systems.add_update_system(std::make_unique<systems::Velocity>());
  ctx.systems.add_update_system(std::make_unique<systems::PositionFollowing>());
  ctx.systems.add_update_system(std::make_unique<systems::Lifetime>());
  ctx.systems.add_update_system(std::make_unique<systems::Animation>());
  ctx.systems.add_update_system(std::make_unique<systems::Score>());
  ctx.systems.add_update_system(std::make_unique<systems::invaders::Eel>(add_eel_entity, rd()));

  ctx.systems.add_draw_system(std::make_unique<systems::Rendering>(ctx.renderer));
  // ctx.systems.add_draw_system(std::make_unique<systems::debug::HitboxRendering>(ctx.renderer));
  ctx.systems.add_draw_system(std::make_unique<systems::HUDRendering>(ctx.renderer));
  ctx.systems.add_draw_system(std::make_unique<systems::Audio>(ctx.audio_player));

  add_player_entity(ctx.ecs);
  add_hud_entity(ctx.ecs);
  add_fortresses(ctx.ecs);

  ctx.ecs.ctx().emplace<components::singleton::Paused>(components::singleton::Paused{
      .paused = false,
      .pause_counter = 0,
  });
}

} // namespace invasion