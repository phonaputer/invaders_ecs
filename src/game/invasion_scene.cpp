#include "game/invasion_scene.hpp"
#include "framework/scene.hpp"
#include "framework/scene_initialization_context.hpp"
#include "game/fortress.hpp"
#include "game/hud.hpp"
#include "game/invaders.hpp"
#include "game/player.hpp"
#include "game/systems/animation.hpp"
#include "game/systems/audio.hpp"
#include "game/systems/collision_detection.hpp"
#include "game/systems/damage.hpp"
#include "game/systems/debug/hitbox_rendering.hpp"
#include "game/systems/deletion.hpp"
#include "game/systems/explode_on_defeat.hpp"
#include "game/systems/hud_rendering.hpp"
#include "game/systems/invaders/orchestration.hpp"
#include "game/systems/lifetime.hpp"
#include "game/systems/player/defeat.hpp"
#include "game/systems/player/movement.hpp"
#include "game/systems/player/shooting.hpp"
#include "game/systems/position_following.hpp"
#include "game/systems/rendering.hpp"
#include "game/systems/score.hpp"
#include "game/systems/sprite_offset_on_damage.hpp"
#include "game/systems/velocity.hpp"
#include <random>

namespace game {

void InvasionScene::initialize(framework::SceneInitializationContext ctx) {
  ctx.assets.load_image_png("invaders_spritesheet", "./assets/space_invaders.png");

  ctx.assets.load_audio_wav("alien_explosion", "./assets/alien_explosion.wav");
  ctx.assets.load_audio_wav("alien_shot", "./assets/alien_shot.wav");
  ctx.assets.load_audio_wav("player_explosion", "./assets/player_explosion.wav");
  ctx.assets.load_audio_wav("player_shot", "./assets/player_shot.wav");
  ctx.assets.load_audio_wav("arp1", "./assets/arp1.wav");
  ctx.assets.load_audio_wav("arp2", "./assets/arp2.wav");
  ctx.assets.load_audio_wav("arp3", "./assets/arp3.wav");
  ctx.assets.load_audio_wav("arp4", "./assets/arp4.wav");

  std::random_device rd;

  ctx.systems.add_update_system(std::make_unique<systems::CollisionDetection>());
  ctx.systems.add_update_system(std::make_unique<systems::Damage>());
  ctx.systems.add_update_system(std::make_unique<systems::SpriteOffsetOnDamage>());
  ctx.systems.add_update_system(
      std::make_unique<systems::player::Defeat>(add_player_explosion_entity, add_player_entity, add_fortresses)
  );
  ctx.systems.add_update_system(std::make_unique<systems::ExplodeOnDefeat>(add_explosion));
  ctx.systems.add_update_system(std::make_unique<systems::Deletion>());
  ctx.systems.add_update_system(std::make_unique<systems::player::Movement>());
  ctx.systems.add_update_system(
      std::make_unique<systems::player::Shooting>(add_player_projectile_entity, add_player_muzzle_flash_entity)
  );
  ctx.systems.add_update_system(
      std::make_unique<systems::invaders::Orchestration>(add_invader_projectile, add_invader_entities, rd())
  );
  ctx.systems.add_update_system(std::make_unique<systems::Velocity>());
  ctx.systems.add_update_system(std::make_unique<systems::PositionFollowing>());
  ctx.systems.add_update_system(std::make_unique<systems::Lifetime>());
  ctx.systems.add_update_system(std::make_unique<systems::Animation>());
  ctx.systems.add_update_system(std::make_unique<systems::Score>());

  ctx.systems.add_draw_system(std::make_unique<systems::Rendering>(ctx.renderer));
  // ctx.systems.add_draw_system(std::make_unique<systems::debug::HitboxRendering>(ctx.renderer));
  ctx.systems.add_draw_system(std::make_unique<systems::HUDRendering>(ctx.renderer));
  ctx.systems.add_draw_system(std::make_unique<systems::Audio>(ctx.audio_player));

  add_invader_entities(ctx.ecs);
  add_player_entity(ctx.ecs);
  add_hud_entity(ctx.ecs);
  add_fortresses(ctx.ecs);
}

} // namespace game