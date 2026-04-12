#include "gallia/test_scene.hpp"
#include "framework/game/scene.hpp"
#include "framework/game/scene_initialization_context.hpp"
#include "gallia/player.hpp"
#include "gallia/systems/player_movement.hpp"
#include "gallia/systems/rendering.hpp"

namespace gallia {

void TestScene::initialize(game::SceneInitializationContext ctx) {
  ctx.assets.load_image_png("invaders_spritesheet", "./assets/space_invaders.png");

  ctx.ecs.add_draw_system(std::make_unique<systems::Rendering>(ctx.renderer));
  ctx.ecs.add_update_system(std::make_unique<systems::PlayerMovement>(ctx.player_input_manager));

  add_player_entity(ctx.ecs);
}

} // namespace gallia