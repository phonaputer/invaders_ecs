#include "game/invasion_scene.hpp"
#include "framework/scene.hpp"
#include "framework/scene_initialization_context.hpp"
#include "game/hud.hpp"
#include "game/invaders.hpp"
#include "game/player.hpp"
#include "game/systems/animation.hpp"
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
#include "game/systems/velocity.hpp"
#include <random>

namespace gallia {

void InvasionScene::initialize(game::SceneInitializationContext ctx) {
  ctx.assets.load_image_png("invaders_spritesheet", "./assets/space_invaders.png");

  std::random_device rd;

  ctx.ecs.add_update_system(std::make_unique<systems::CollisionDetection>());
  ctx.ecs.add_update_system(std::make_unique<systems::Damage>());
  ctx.ecs.add_update_system(std::make_unique<systems::player::Defeat>(add_player_explosion_entity, add_player_entity));
  ctx.ecs.add_update_system(std::make_unique<systems::ExplodeOnDefeat>(add_explosion));
  ctx.ecs.add_update_system(std::make_unique<systems::Deletion>());
  ctx.ecs.add_update_system(std::make_unique<systems::player::Movement>(ctx.player_input_manager));
  ctx.ecs.add_update_system(
      std::make_unique<systems::player::Shooting>(
          ctx.player_input_manager, add_player_projectile_entity, add_player_muzzle_flash_entity
      )
  );
  ctx.ecs.add_update_system(
      std::make_unique<systems::invaders::Orchestration>(add_invader_projectile, add_invader_entities, rd())
  );
  ctx.ecs.add_update_system(std::make_unique<systems::Velocity>());
  ctx.ecs.add_update_system(std::make_unique<systems::PositionFollowing>());
  ctx.ecs.add_update_system(std::make_unique<systems::Lifetime>());
  ctx.ecs.add_update_system(std::make_unique<systems::Animation>());
  ctx.ecs.add_update_system(std::make_unique<systems::Score>());

  ctx.ecs.add_draw_system(std::make_unique<systems::Rendering>(ctx.renderer));
  // ctx.ecs.add_draw_system(std::make_unique<systems::debug::HitboxRendering>(ctx.renderer));
  ctx.ecs.add_draw_system(std::make_unique<systems::HUDRendering>(ctx.renderer));

  add_invader_entities(ctx.ecs);
  add_player_entity(ctx.ecs);
  add_hud_entity(ctx.ecs);
}

} // namespace gallia