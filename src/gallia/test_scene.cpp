#include "gallia/test_scene.hpp"
#include "framework/game/scene.hpp"
#include "framework/game/scene_initialization_context.hpp"
#include "gallia/hud.hpp"
#include "gallia/invaders.hpp"
#include "gallia/player.hpp"
#include "gallia/systems/animation.hpp"
#include "gallia/systems/collision_detection.hpp"
#include "gallia/systems/damage.hpp"
#include "gallia/systems/debug/hitbox_rendering.hpp"
#include "gallia/systems/deletion.hpp"
#include "gallia/systems/hud_rendering.hpp"
#include "gallia/systems/invaders/orchestration.hpp"
#include "gallia/systems/lifetime.hpp"
#include "gallia/systems/player/movement.hpp"
#include "gallia/systems/player/shooting.hpp"
#include "gallia/systems/position_following.hpp"
#include "gallia/systems/rendering.hpp"
#include "gallia/systems/score.hpp"
#include "gallia/systems/velocity.hpp"
#include <random>

namespace gallia {

void TestScene::initialize(game::SceneInitializationContext ctx) {
  ctx.assets.load_image_png("invaders_spritesheet", "./assets/space_invaders.png");

  std::random_device rd;

  ctx.ecs.add_update_system(std::make_unique<systems::CollisionDetection>());
  ctx.ecs.add_update_system(std::make_unique<systems::Damage>());
  ctx.ecs.add_update_system(std::make_unique<systems::Deletion>());
  ctx.ecs.add_update_system(std::make_unique<systems::player::Movement>(ctx.player_input_manager));
  ctx.ecs.add_update_system(
      std::make_unique<systems::player::Shooting>(
          ctx.player_input_manager, add_player_projectile_entity, add_player_muzzle_flash_entity
      )
  );
  ctx.ecs.add_update_system(std::make_unique<systems::invaders::Orchestration>(add_invader_projectile, rd()));
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